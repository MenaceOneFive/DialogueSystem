#include "Graph/DialogueGraphEditor.h"

#include "DialogueGraphBlueprintExtension.h"
#include "DialogueSystemRuntimeModule.h"
#include "EdGraphNode_Comment.h"
#include "EdGraphUtilities.h"
#include "GraphEditor.h"
#include "PropertyEditorModule.h"
#include "SGraphPanel.h"
#include "Blueprint/BlueprintExtension.h"
#include "Character/DialogueCharacterAsset.h"
#include "EdGraph/EdGraph.h"
#include "EdGraph/EdGraphNode.h"
#include "Framework/Commands/GenericCommands.h"
#include "Framework/Docking/TabManager.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Graph/DialogueGraph.h"
#include "Graph/DialogueGraphEditorCommands.h"
#include "Graph/Node/DialogueEdGraphDialogueLineNode.h"
#include "Graph/Node/DialogueEdGraphEndNode.h"
#include "Graph/Node/DialogueEdGraphKnotNode.h"
#include "Graph/Node/DialogueEdGraphNode.h"
#include "Graph/Node/DialogueEdGraphSelectNode.h"
#include "Graph/Node/DialogueEdGraphStartNode.h"
#include "Graph/Node/DialogueGraphNode.h"
#include "Graph/Node/DialogueSelectionNode.h"
#include "Graph/Node/DialogueStartNode.h"
#include "Graph/Slate/PropertyEditor/DialogueCharacterDetail.h"
#include "Graph/Slate/PropertyEditor/DialogueGraphDetail.h"
#include "Graph/Slate/PropertyEditor/DialogueLineCreationPromptDetail.h"
#include "Graph/Slate/PropertyEditor/DialogueLineNodeDetail.h"
#include "Graph/Slate/PropertyEditor/DialogueSelectionNodeDetail.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Modules/ModuleManager.h"
#include "Visitor/DialogueEdGraphConnector.h"
#include "Visitor/DialogueEdGraphLogger.h"
#include "Visitor/DialogueEdGraphNodeCreator.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Notifications/SNotificationList.h"

#define LOCTEXT_NAMESPACE "FDialogueGraphEditor"

namespace DialogueGraphEditorTabs // 탭 ID 정의
{
    static const FName GraphEditorID = TEXT("DialogueGraphEditor_GraphEditor");
    static const FName DetailsID     = TEXT("DialogueGraphEditor_Details");
}

static const FName DialogueGraphEditorAppName = FName(TEXT("DialogueGraphEditorApp"));

/// <summary>
/// 라이프사이클 관련 생성자, 소멸자, 초기화 함수 정의
/// </summary>
#pragma region 라이프사이클

FDialogueGraphEditor::FDialogueGraphEditor()
    : DialogueGraphAsset(nullptr)
{
    // 에디터 엔진에 Undo 클라이언트로 등록
    GEditor->RegisterForUndo(this);
}

FDialogueGraphEditor::~FDialogueGraphEditor()
{
    // Undo 클라이언트 등록 해제
    GEditor->UnregisterForUndo(this);
    if (DialogueGraphAsset->IsValidLowLevel())
    {
        auto Extension = DialogueGraphAsset->GetBlueprintInstance()->GetExtensions().FindByPredicate(
                                                                                                     [](TObjectPtr<UBlueprintExtension> Extention)
                                                                                                     {
                                                                                                         return Extention->IsA<UDialogueGraphBlueprintExtension>();
                                                                                                     });
        if (Extension)
        {
            DialogueGraphAsset->GetBlueprintInstance()->RemoveExtension(*Extension);
        }
    }
}

void FDialogueGraphEditor::InitDialogueGraphEditor(const EToolkitMode::Type        Mode,
                                                   const TSharedPtr<IToolkitHost>& InitToolkitHost,
                                                   UDialogueGraph*                 InDialogueGraph)
{
    // 그래프 에셋 설정
    DialogueGraphAsset   = InDialogueGraph;
    const auto Blueprint = DialogueGraphAsset->GetBlueprintInstance();

    if (OnChangedHandle.IsValid())
    {
        Blueprint->OnChanged().Remove(OnChangedHandle);
        OnChangedHandle.Reset();
    }

    OnChangedHandle = Blueprint->OnChanged().AddStatic(&FDialogueGraphEditorUtils::OnBlueprintChanged);

    checkf(Blueprint, TEXT("DialogueGraphAsset에 Blueprint가 존재하지 않습니다."));

    const bool ContainsExtension = DialogueGraphAsset->GetBlueprintInstance()
                                                     ->GetExtensions()
                                                      .ContainsByPredicate([](TObjectPtr<UBlueprintExtension> Extention)
                                                       {
                                                           return Extention->IsA<UDialogueGraphBlueprintExtension>();
                                                       });

    if (!ContainsExtension)
    {
        const auto DialogueBlueprintExtension = NewObject<UDialogueGraphBlueprintExtension>(Blueprint);
        Blueprint->AddExtension(DialogueBlueprintExtension);
    }

    OnCompiledHandle = Blueprint->OnCompiled().AddSP(this, &FDialogueGraphEditor::OnBlueprintRecompiled, DialogueGraphAsset);

    // 그래프 에디터 명령어 초기화
    if (!GraphEditorCommands.IsValid())
    {
        GraphEditorCommands = MakeShareable(new FUICommandList);

        // 여기에 그래프 에디터 명령어 매핑 추가
        GraphEditorCommands->MapAction(FDialogueGraphEditorCommands::Get().Command1,
                                       FExecuteAction::CreateSP(this, &FDialogueGraphEditor::OnCreateComment),
                                       FCanExecuteAction::CreateLambda([]() { return true; }));
        GraphEditorCommands->MapAction(FDialogueGraphEditorCommands::Get().CommentSelection,
                                       FExecuteAction::CreateSP(this, &FDialogueGraphEditor::OnCreateComment),
                                       FCanExecuteAction::CreateLambda([this]()
                                       {
                                           return !this->GetSelectedNodes().IsEmpty();
                                       }));

        // 노드 삭제 명령
        GraphEditorCommands->MapAction(FDialogueGraphEditorCommands::Get().DeleteSelectedNode,
                                       FExecuteAction::CreateSP(this, &FDialogueGraphEditor::DeleteSelectedNode),
                                       FCanExecuteAction::CreateSP(this, &FDialogueGraphEditor::CanExecuteDeleteSelectedNode));

        // 연결 삭제 명령
        GraphEditorCommands->MapAction(FDialogueGraphEditorCommands::Get().DeleteAllNodeConnection,
                                       FExecuteAction::CreateSP(this, &FDialogueGraphEditor::DeleteAllNodeConnection),
                                       FCanExecuteAction::CreateSP(this, &FDialogueGraphEditor::CanDeleteAllNodeConnection));

        // Delete가 눌린 경우 -> 노드를 삭제함
        GraphEditorCommands->MapAction(FGenericCommands::Get().Delete,
                                       FExecuteAction::CreateSP(this, &FDialogueGraphEditor::OnDeleteKeyPressed));

        // 잘라내기, 복사, 붙여넣기, 복제 명령
        GraphEditorCommands->MapAction(FGenericCommands::Get().Cut,
                                       FExecuteAction::CreateSP(this, &FDialogueGraphEditor::CutSelectedNodes),
                                       FCanExecuteAction::CreateSP(this, &FDialogueGraphEditor::CanCutNodes)
                                      );

        GraphEditorCommands->MapAction(FGenericCommands::Get().Copy,
                                       FExecuteAction::CreateSP(this, &FDialogueGraphEditor::CopySelectedNodes),
                                       FCanExecuteAction::CreateSP(this, &FDialogueGraphEditor::CanCopyNodes)
                                      );

        GraphEditorCommands->MapAction(FGenericCommands::Get().Paste,
                                       FExecuteAction::CreateSP(this, &FDialogueGraphEditor::PasteNodes),
                                       FCanExecuteAction::CreateSP(this, &FDialogueGraphEditor::CanPasteNodes)
                                      );

        GraphEditorCommands->MapAction(FGenericCommands::Get().Duplicate,
                                       FExecuteAction::CreateSP(this, &FDialogueGraphEditor::DuplicateNodes),
                                       FCanExecuteAction::CreateSP(this, &FDialogueGraphEditor::CanDuplicateNodes)
                                      );
    }


    //FDialogueGraphEditor::CreateGraphEditorWidget
    const TSharedRef<FTabManager::FLayout> DefaultLayout = FTabManager::NewLayout("DefaultDialogueGraphEditorLayout")
          ->AddArea
            (FTabManager::NewPrimaryArea()
           ->SetOrientation(Orient_Vertical)
           ->Split(FTabManager::NewSplitter()
                 ->SetOrientation(Orient_Horizontal)
                 ->SetSizeCoefficient(0.9f)
                 ->Split(FTabManager::NewStack()
                       ->SetSizeCoefficient(0.7f)
                       ->AddTab(DialogueGraphEditorTabs::GraphEditorID, ETabState::OpenedTab))
                 ->Split(FTabManager::NewStack()
                       ->SetSizeCoefficient(0.3f)
                       ->AddTab(DialogueGraphEditorTabs::DetailsID, ETabState::OpenedTab)
                       ->AddTab(FName("YourWidgetTabID"), ETabState::OpenedTab)
                        )
                  )
            );

    // 에셋 에디터 초기화
    constexpr bool bCreateDefaultStandaloneMenu = true;
    constexpr bool bCreateDefaultToolbar        = true;

    InitAssetEditor(Mode,
                    InitToolkitHost,
                    DialogueGraphEditorAppName,
                    DefaultLayout, // <- 여기에 위젯 구성
                    bCreateDefaultStandaloneMenu,
                    bCreateDefaultToolbar,
                    InDialogueGraph);
}

#pragma endregion

/// <summary>
/// FWorkflowCentricApplication 인터페이스 구현 (탭 등록/해제 등)
/// </summary>
#pragma region FWorkflowCentricApplication_Implementation_(탭_구현)

void FDialogueGraphEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
    FWorkflowCentricApplication::RegisterTabSpawners(InTabManager);
    // 그래프 패널
    InTabManager->RegisterTabSpawner(DialogueGraphEditorTabs::GraphEditorID,
                                     FOnSpawnTab::CreateSP(this, &FDialogueGraphEditor::GraphEditorTabSpawner))
                 .SetDisplayName(LOCTEXT("GraphTab", "Graph"))
                 .SetGroup(GetWorkspaceMenuCategory())
                 .SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "GraphEditor.EventGraph_16x"));

    // 속성 편집기
    InTabManager->RegisterTabSpawner(DialogueGraphEditorTabs::DetailsID,
                                     FOnSpawnTab::CreateSP(this, &FDialogueGraphEditor::PropertyTabSpawner))
                 .SetDisplayName(LOCTEXT("Details", "Detail"))
                 .SetGroup(GetWorkspaceMenuCategory());
}

void FDialogueGraphEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
    FWorkflowCentricApplication::UnregisterTabSpawners(InTabManager);
}

TSharedRef<SDockTab> FDialogueGraphEditor::GraphEditorTabSpawner(const FSpawnTabArgs& SpawnTabArgs)
{
    UDialogueEdGraph* DialogueEdGraph = Cast<UDialogueEdGraphAssetData>(DialogueGraphAsset->AssetUserData)->DialogueEdGraph;

    check(DialogueEdGraph) // 가능한 경우 : DialogueGraphAsset에서 DialogueGraph가 적절히 초기화되지 않았음

    const TSharedRef<SGraphEditor> GraphWidget = CreateGraphEditorWidget(DialogueEdGraph);

    FocusedGraphEdPtr = GraphWidget;

    auto DockTab = SNew(SDockTab)
                                .TabRole(PanelTab)
                                .CanEverClose(false) // 나중에 다시 열기 버튼이 생기면 제거
        [
            SNew(SOverlay)
            + SOverlay::Slot()
             .HAlign(HAlign_Fill)
             .VAlign(VAlign_Fill)
            [
                GraphWidget
            ]
            + SOverlay::Slot()
             .HAlign(HAlign_Center)
             .VAlign(VAlign_Bottom)
             .Padding(FMargin{0.f, 0.f, 0.f, 60.f})
             .Expose(CurrentLineOverlay)
            [
                SAssignNew(Tb_CurrentLine, STextBlock)
               .Text(FText::FromString("Hello world"))
            ]
        ];
    HideSubtitle();
    return DockTab;
}

TSharedRef<SDockTab> FDialogueGraphEditor::PropertyTabSpawner(const FSpawnTabArgs& Args)
{
    // 속성 에디터 생성
    FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
    FDetailsViewArgs       DetailsViewArgs;
    DetailsViewArgs.bUpdatesFromSelection = false;
    DetailsViewArgs.bLockable             = false;
    DetailsViewArgs.bAllowSearch          = true;
    DetailsViewArgs.NameAreaSettings      = FDetailsViewArgs::ObjectsUseNameArea;
    DetailsViewArgs.bHideSelectionTip     = false;
    PropertyEditor                        = PropertyEditorModule.CreateDetailView(DetailsViewArgs);

    #pragma region DetailTypeCustomization // DetailTypeCustomization를 등록

    // 프롬프트 
    PropertyEditor->RegisterInstancedCustomPropertyTypeLayout(FDialogueLineCreationPrompt::StaticStruct()->GetFName(),
                                                              FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FDialogueLineCreationPromptDetailTypeCustomization::MakeInstance));

    // 그래프
    PropertyEditor->RegisterInstancedCustomPropertyLayout(UDialogueGraph::StaticClass(), FOnGetDetailCustomizationInstance::CreateStatic(&FDialogueGraphDetail::MakeInstance));


    //캐릭터
    PropertyEditor->RegisterInstancedCustomPropertyTypeLayout(UDialogueCharacterAsset::StaticClass()->GetFName(),
                                                              FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FDialogueCharacterDetail::MakeInstance));

    // 대사노드
    PropertyEditor->RegisterInstancedCustomPropertyLayout(UDialogueEdGraphDialogueLineNode::StaticClass(), FOnGetDetailCustomizationInstance::CreateStatic(&FDialogueLineNodeDetail::MakeInstance));

    // 선택노드
    PropertyEditor->RegisterInstancedCustomPropertyLayout(UDialogueEdGraphSelectNode::StaticClass(), FOnGetDetailCustomizationInstance::CreateStatic(&FDialogueSelectionNodeDetail::MakeInstance));

    // 시작노드 (기본 Node용 디테일 사용)
    PropertyEditor->RegisterInstancedCustomPropertyLayout(UDialogueEdGraphStartNode::StaticClass(), FOnGetDetailCustomizationInstance::CreateStatic(&FDialogueNodeDetail::MakeInstance));

    // 종료노드 (기본 Node용 디테일 사용)
    PropertyEditor->RegisterInstancedCustomPropertyLayout(UDialogueEdGraphEndNode::StaticClass(), FOnGetDetailCustomizationInstance::CreateStatic(&FDialogueNodeDetail::MakeInstance));

    #pragma endregion

    // 처음 편집할 데이터
    PropertyEditor->SetObject(DialogueGraphAsset);

    TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab)
                                                   .Label(LOCTEXT("DetailsPanel", "Details"))
                                                   .CanEverClose(false) // 나중에 다시 열기 버튼이 만들어지면 이 라인을 삭제
        [
            PropertyEditor.ToSharedRef()
        ];

    return SpawnedTab;
}

#pragma endregion

/// <summary>
/// IToolkit 인터페이스 구현 (Toolkit 이름, 툴팁 등)
/// </summary>
#pragma region IToolkit_Implementation

FName FDialogueGraphEditor::GetToolkitFName() const
{
    return FName("DialogueGraphEditor");
}

FText FDialogueGraphEditor::GetBaseToolkitName() const
{
    return NSLOCTEXT("DialogueGraphEditor", "AppLabel", "Dialogue Graph Editor");
}

FText FDialogueGraphEditor::GetToolkitName() const
{
    return FText::FromString(DialogueGraphAsset->GetName());
}

FText FDialogueGraphEditor::GetToolkitToolTipText() const
{
    return NSLOCTEXT("DialogueGraphEditor", "ToolTip", "Dialogue Graph Editor");
}

FString FDialogueGraphEditor::GetWorldCentricTabPrefix() const
{
    return TEXT("DialogueGraphEditor");
}

FLinearColor FDialogueGraphEditor::GetWorldCentricTabColorScale() const
{
    return FLinearColor(0.3f, 0.4f, 0.7f, 0.5f);
}

#pragma endregion

/// <summary>
/// FEditorUndoClient 인터페이스 구현 (Undo/Redo 후 UI 업데이트)
/// </summary>
#pragma region FEditorUndoClient_Implementation

void FDialogueGraphEditor::PostUndo(bool bSuccess)
{
    // Undo 실행 후 UI 업데이트
    if (bSuccess)
    {
        // 그래프 에디터 위젯 새로고침
        if (FocusedGraphEdPtr.IsValid())
        {
            FocusedGraphEdPtr.Pin()->NotifyGraphChanged();
        }

        // 속성 에디터 새로고침
        PropertyEditor->ForceRefresh();
    }
}

void FDialogueGraphEditor::PostRedo(bool bSuccess)
{
    // Redo 실행 후 UI 업데이트
    if (bSuccess)
    {
        // 그래프 에디터 위젯 새로고침
        if (FocusedGraphEdPtr.IsValid())
        {
            FocusedGraphEdPtr.Pin()->NotifyGraphChanged();
        }

        // 속성 에디터 새로고침
        PropertyEditor->ForceRefresh();
    }
}

void FDialogueGraphEditor::OnNodeTitleCommitted(const FText& Text, ETextCommit::Type Arg, UEdGraphNode* EdGraphNode) const
{
    if (const auto CommentNode = Cast<UEdGraphNode_Comment>(EdGraphNode))
    {
        const FScopedTransaction Transaction(NSLOCTEXT("DialogueGraph_RenameNode", "RenameNode", "Rename Node"));
        CommentNode->Modify();
        CommentNode->OnRenameNode(Text.ToString());
    }
}

#pragma endregion

/// <summary>
/// 에디터 윈도우 이벤트 리스너 (Delete 키 이벤트 등)
/// </summary>
#pragma region 에디터_윈도우_이벤트_리스너

void FDialogueGraphEditor::OnDeleteKeyPressed() const
{
    const FScopedTransaction Transaction(LOCTEXT("DeleteNode", "Delete node from graph"));
    if (const TSharedPtr<SGraphEditor>
        GraphEditor = FocusedGraphEdPtr.Pin())
    {
        RemoveSelectedNodesFrom(GraphEditor);
    }
}

#pragma endregion

/// <summary>
/// 그래프 에디터 위젯 팩토리 메서드
/// </summary>
#pragma region 위젯_팩토리_메서드

void FDialogueGraphEditor::OnBlueprintRecompiled(UBlueprint*     Blueprint,
                                                 UDialogueGraph* Asset)
{
    check(Asset->IsValidLowLevel())
    // TODO : 블루프린트가 DialogueGraph의 블루프린트 디렉터 클래스를 상속하는지 체크할 것
    Blueprint->Modify();
    Asset->Modify();

    // 기존 델리게이트를 제거
    UBlueprint* OldBlueprint = Asset->GetBlueprintInstance();

    if (OnCompiledHandle.IsValid())
    {
        OldBlueprint->OnCompiled().Remove(OnCompiledHandle);
        OnCompiledHandle.Reset();
    }

    if (OnChangedHandle.IsValid())
    {
        OldBlueprint->OnChanged().Remove(OnChangedHandle);
        OnChangedHandle.Reset();
    }

    // 새 델리게이트를 설정
    OnCompiledHandle = Blueprint->OnCompiled().AddSP(this, &FDialogueGraphEditor::OnBlueprintRecompiled, DialogueGraphAsset);
    OnChangedHandle  = Blueprint->OnChanged().AddStatic(&FDialogueGraphEditorUtils::OnBlueprintChanged);
    Asset->SetBlueprintInstance(Blueprint);
}

TSharedRef<SGraphEditor> FDialogueGraphEditor::CreateGraphEditorWidget(UDialogueEdGraph* InGraph)
{
    // 그래프 에디터 이벤트 설정
    SGraphEditor::FGraphEditorEvents GraphEvents;
    SetupGraphEditorEvents(InGraph, GraphEvents);

    // 그래프 에디터 위젯 생성
    TSharedRef<SGraphEditor> GraphEditor = SNew(SGraphEditor)
                                                            .AdditionalCommands(GraphEditorCommands)
                                                            .GraphToEdit(InGraph)
                                                            .GraphEvents(GraphEvents);

    // 포커스된 그래프 에디터 설정
    FocusedGraphEdPtr = GraphEditor;

    return GraphEditor;
}

#pragma endregion

/// <summary>
/// 그래프 에디터 위젯 이벤트 리스너 (노드 선택 변경, 더블 클릭 등)
/// </summary>
#pragma region 그래프_에디터_위젯_이벤트_리스너

void FDialogueGraphEditor::OnCreateComment() const
{
    // 코멘트 노드 생성 로직
    if (const auto GraphEditor = this->FocusedGraphEdPtr.Pin())
    {
        const auto Graph                   = GraphEditor->GetCurrentGraph();
        const auto CommentNode             = NewObject<UEdGraphNode_Comment>(Graph);
        CommentNode->bCanRenameNode        = true;
        CommentNode->bCommentBubbleVisible = false;

        // 현재 선택한 노드들의 영역을 산출
        FSlateRect BoundSize;
        GraphEditor->GetBoundsForSelectedNodes(BoundSize, 50.f);
        CommentNode->SetBounds(BoundSize);

        CommentNode->NodeComment = "Comment";

        // 최종적으로 생성된 노드를 그래프에 더한다.
        Graph->AddNode(CommentNode);

        Graph->NotifyNodeChanged(CommentNode);
        Graph->NotifyGraphChanged();
    }
}


void FDialogueGraphEditor::GenerateRuntimeGraph() const
{
    const auto        GraphData     = Cast<UDialogueEdGraphAssetData>(DialogueGraphAsset->AssetUserData);
    UDialogueEdGraph* EditorGraph   = GraphData->DialogueEdGraph;
    UDialogueGraph*   DialogueGraph = DialogueGraphAsset;

    // 기존의 런타임 노드들을 제거한다.
    DialogueGraph->Clear();

    TMap<UDialogueEdGraphNode*, UDialogueGraphNode*> EditorToRuntime;

    const TUniquePtr<FAbstractDialogueEdGraphVisitor> Creator =
            MakeUnique<FDialogueEdGraphNodeCreator>(DialogueGraph, &EditorToRuntime);

    const TUniquePtr<FAbstractDialogueEdGraphVisitor> Connector =
            MakeUnique<FDialogueEdGraphConnector>(&EditorToRuntime);

    const TUniquePtr<FAbstractDialogueEdGraphVisitor> Logger =
            MakeUnique<FDialogueEdGraphLogger>(&EditorToRuntime);

    // 노드 유형별로 초기화한 뒤 EditorToRuntime에 저장
    for (const TObjectPtr EdGraphNode : EditorGraph->Nodes)
    {
        // 생략해야 하는 노드 유형은 여기을 여기에 추가
        if (EdGraphNode->IsA<UDialogueEdGraphKnotNode>() // 노트 노드인 경우
            || EdGraphNode->IsA<UEdGraphNode_Comment>()  // 주석 노드인 경우
        )
        {
            // 별도의 작업을 수행하지 않는다.
            continue;
        }

        if (const auto DialogueEdGraphNode = Cast<UDialogueEdGraphNode>(EdGraphNode))
        {
            DialogueEdGraphNode->Accept(Creator.Get());

            // 시작 노드인 경우 에셋 그래프의 시작점으로 지정해야 한다.
            if (DialogueEdGraphNode->IsA<UDialogueEdGraphStartNode>())
            {
                DialogueGraph->SetStartNode(Cast<UDialogueStartNode>(EditorToRuntime[DialogueEdGraphNode]));
            }
        }
    }

    // 개별 노드를 순회하면서 연결 정보 갱신
    for (const TObjectPtr EdGraphNode : EditorGraph->Nodes)
    {
        if (const auto DialogueEdGraphNode = Cast<UDialogueEdGraphNode>(EdGraphNode);
            DialogueEdGraphNode && !DialogueEdGraphNode->IsA<UDialogueEdGraphKnotNode>())
        {
            DialogueEdGraphNode->Accept(Connector.Get());
        }
    }

    // 연결 관계를 UE_LOG로 출력합니다.
    for (const TObjectPtr EdGraphNode : EditorGraph->Nodes)
    {
        if (const auto DialogueEdGraphNode = Cast<UDialogueEdGraphNode>(EdGraphNode);
            DialogueEdGraphNode && !DialogueEdGraphNode->IsA<UDialogueEdGraphKnotNode>())
        {
            DialogueEdGraphNode->Accept(Logger.Get());
        }
    }
}

void FDialogueGraphEditor::SaveAsset_Execute()
{
    GenerateRuntimeGraph();
    FWorkflowCentricApplication::SaveAsset_Execute();
}

void FDialogueGraphEditor::DeleteSelectedNode() const
{
    if (const TSharedPtr<SGraphEditor> GraphEditor = this->FocusedGraphEdPtr.Pin())
    {
        RemoveSelectedNodesFrom(GraphEditor);
    }
}

bool FDialogueGraphEditor::CanExecuteDeleteSelectedNode() const
{
    if (TSharedPtr<SGraphEditor> GraphEditor = this->FocusedGraphEdPtr.Pin())
    {
        if (auto SelectedNodes = GraphEditor->GetSelectedNodes();
            SelectedNodes.Num() > 0)
        {
            // 시작노드만 선택한 경우에는 삭제할 수 없음
            if (SelectedNodes.Num() == 1 && Cast<UDialogueEdGraphStartNode>(*SelectedNodes.begin()))
            {
                return false;
            }
            // 그게 아니면 삭제할 수 있음
            return true;
        }
    }
    return false;
}

void FDialogueGraphEditor::DeleteAllNodeConnection() const
{
    const TSharedPtr<SGraphEditor> Editor = this->FocusedGraphEdPtr.Pin();
    if (!Editor.IsValid())
    {
        return;
    }

    // 선택된 노드가 없다면 실행할 수 없다.
    const auto SelectedNodes = Editor->GetSelectedNodes();
    if (SelectedNodes.IsEmpty())
    {
        return;
    }

    for (const auto SelectedNode : SelectedNodes)
    {
        if (UDialogueEdGraphNode* Node = Cast<UDialogueEdGraphNode>(SelectedNode))
        {
            FScopedTransaction Transaction{FText::FromString("Delete All Node Connection")};
            for (UEdGraphPin* Pin : Node->Pins)
            {
                Pin->BreakAllPinLinks(true);
            }
        }
    }
}

bool FDialogueGraphEditor::CanDeleteAllNodeConnection() const
{
    const TSharedPtr<SGraphEditor> Editor = this->FocusedGraphEdPtr.Pin();
    if (!Editor.IsValid())
    {
        return false;
    }

    // 선택된 노드가 없다면 실행할 수 없다.
    const auto SelectedNodes = Editor->GetSelectedNodes();
    if (SelectedNodes.IsEmpty())
    {
        return false;
    }

    // 선택된 노드 중에 연결된 노드가 존재해야 한다.
    const auto Result = Algo::FindByPredicate(SelectedNodes, [](UObject* SelectedNode)
    {
        if (const auto EdGraphNode = Cast<UDialogueEdGraphNode>(SelectedNode))
        {
            for (const UEdGraphPin* Pin : EdGraphNode->Pins)
            {
                if (Pin->HasAnyConnections())
                {
                    return true;
                }
            }
        }
        return false;
    });
    return Result != nullptr;
}


void FDialogueGraphEditor::SetupGraphEditorEvents(UDialogueEdGraph*                 InGraph,
                                                  SGraphEditor::FGraphEditorEvents& InEvents)
{
    // 노드 선택 변경 이벤트 설정
    InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FDialogueGraphEditor::OnSelectedNodesChanged);
    InEvents.OnDoubleClicked    = SGraphEditor::FOnDoubleClicked::CreateSP(this, &FDialogueGraphEditor::OnDoubleClicked);
    InEvents.OnTextCommitted    = FOnNodeTextCommitted::CreateSP(this, &FDialogueGraphEditor::OnNodeTitleCommitted);
}

void FDialogueGraphEditor::OnSelectedNodesChanged(const TSet<UObject*>& NewSelection) const
{
    // 선택된 노드에 따라 속성 에디터 업데이트
    if (NewSelection.Num() == 1)
    {
        for (UObject* SelectionObj : NewSelection)
        {
            if (!SelectionObj->IsA<UDialogueEdGraphNode>())
            {
                // 코멘트 노드처럼 완전히 UDialogueEdGraphNode가 아닌 경우 
                continue;
            }

            if (SelectionObj)
            {
                PropertyEditor->SetObject(SelectionObj);
                if (auto DialogueLineNode = Cast<UDialogueEdGraphDialogueLineNode>(SelectionObj))
                {
                    if (DialogueLineNode->DialogueCharacterAsset)
                    {
                        SetSubtitleText(FString::Format(TEXT("{0} : {1}"),
                                                        {DialogueLineNode->DialogueCharacterAsset->Name, DialogueLineNode->DialogueLine}));
                    }
                    return;
                }
                HideSubtitle();
                return;
            }
        }
    }

    // 선택된 노드가 없거나 여러 개인 경우 대화 그래프 에셋 표시
    PropertyEditor->SetObject(DialogueGraphAsset);
    HideSubtitle();
}

void FDialogueGraphEditor::OnDoubleClicked() const
{
}

#pragma endregion

void FDialogueGraphEditor::RemoveSelectedNodesFrom(const TSharedPtr<SGraphEditor>& GraphEditor) const
{
    UEdGraph*      FocusedGraph = GraphEditor->GetCurrentGraph();
    TSet<UObject*> Objects      = GraphEditor->GetSelectedNodes();
    for (UObject* SelectedNode : Objects)
    {
        if (SelectedNode->IsA<UDialogueEdGraphStartNode>())
        {
            FNotificationInfo Info(LOCTEXT("CannotRemoveStartNode", "시작노드는 삭제할 수 없습니다."));
            Info.ExpireDuration = 2.0f;
            FSlateNotificationManager::Get().AddNotification(Info);
            continue; // 어떠한 경우에도 시작노드는 삭제할 수 없다.
        }
        UEdGraphNode* NodeToRemove = Cast<UEdGraphNode>(SelectedNode);
        FocusedGraph->RemoveNode(NodeToRemove);
    }
}

#pragma region CopyPaste

TSet<UObject*> FDialogueGraphEditor::GetSelectedNodes() const
{
    TSet<UObject*> SelectedNodes;
    if (const TSharedPtr<SGraphEditor> FocusedGraphEd = FocusedGraphEdPtr.Pin())
    {
        SelectedNodes = FocusedGraphEd->GetSelectedNodes();
    }
    return SelectedNodes;
}

void FDialogueGraphEditor::CopySelectedNodes() const
{
    // 선택된 노드를 텍스트로 내보내 클립보드에 복사합니다.
    const TSet<UObject*> SelectedNodes = GetSelectedNodes();
    if (SelectedNodes.IsEmpty())
    {
        return;
    }

    FString ExportedText;
    FEdGraphUtilities::ExportNodesToText(SelectedNodes, ExportedText);
    FPlatformApplicationMisc::ClipboardCopy(*ExportedText);
}

bool FDialogueGraphEditor::CanCopyNodes() const
{
    // 복제 가능한 노드가 하나라도 선택되어 있으면 복사할 수 있습니다.
    const TSet<UObject*> SelectedNodes = GetSelectedNodes();
    if (SelectedNodes.IsEmpty())
    {
        return false;
    }

    for (UObject* NodeObject : SelectedNodes)
    {
        if (const UEdGraphNode* Node = Cast<UEdGraphNode>(NodeObject))
        {
            if (Node->CanDuplicateNode())
            {
                return true;
            }
        }
    }
    return false;
}

void FDialogueGraphEditor::PasteNodes() const
{
    if (const TSharedPtr<SGraphEditor> FocusedGraphEd = FocusedGraphEdPtr.Pin())
    {
        const FVector2D PasteLocation = FocusedGraphEd->GetCurrentGraph()->GetGoodPlaceForNewNode();
        UEdGraph*       EdGraph       = FocusedGraphEd->GetCurrentGraph();

        const FScopedTransaction Transaction(LOCTEXT("PasteNode", "Paste Node from Clipboard"));
        EdGraph->Modify();

        // 클립보드에서 텍스트를 가져옵니다.
        FString ClipboardContent;
        FPlatformApplicationMisc::ClipboardPaste(ClipboardContent);

        // 붙여넣을 노드 집합을 만듭니다.
        TSet<UEdGraphNode*> PastedNodes;
        FEdGraphUtilities::ImportNodesFromText(EdGraph, ClipboardContent, PastedNodes);

        FVector2D AvgNodePosition(0.0f, 0.0f);
        for (UEdGraphNode* Node : PastedNodes)
        {
            // Node->AllocateDefaultPins();

            // 붙여넣은 노드의 평균 위치를 계산합니다.
            AvgNodePosition.X += Node->NodePosX;
            AvgNodePosition.Y += Node->NodePosY;
        }

        if (!PastedNodes.IsEmpty())
        {
            AvgNodePosition /= static_cast<float>(PastedNodes.Num());
        }

        // 붙여넣은 노드를 지정된 위치로 이동시킵니다.
        for (UEdGraphNode* Node : PastedNodes)
        {
            Node->NodePosX = (Node->NodePosX - AvgNodePosition.X) + PasteLocation.X;
            Node->NodePosY = (Node->NodePosY - AvgNodePosition.Y) + PasteLocation.Y;
            Node->SnapToGrid(16); // 그리드에 맞춤
        }

        // 붙여넣은 노드를 선택하고 그래프 뷰를 업데이트합니다.
        FocusedGraphEd->ClearSelectionSet();
        for (UEdGraphNode* Node : PastedNodes)
        {
            FocusedGraphEd->SetNodeSelection(Node, true);
        }
        FocusedGraphEd->NotifyGraphChanged();
    }
}

bool FDialogueGraphEditor::CanPasteNodes() const
{
    if (const TSharedPtr<SGraphEditor> FocusedGraphEd = FocusedGraphEdPtr.Pin())
    {
        FString ClipboardContent;
        FPlatformApplicationMisc::ClipboardPaste(ClipboardContent);
        return FEdGraphUtilities::CanImportNodesFromText(FocusedGraphEd->GetCurrentGraph(), ClipboardContent);
    }
    return false;
}

void FDialogueGraphEditor::CutSelectedNodes() const
{
    CopySelectedNodes();
    DeleteSelectedNode(); // 기존의 삭제 로직 재사용
}

bool FDialogueGraphEditor::CanCutNodes() const
{
    return CanCopyNodes() && CanExecuteDeleteSelectedNode();
}

void FDialogueGraphEditor::DuplicateNodes() const
{
    CopySelectedNodes();
    PasteNodes();
}

bool FDialogueGraphEditor::CanDuplicateNodes() const
{
    return CanCopyNodes();
}

#pragma endregion

#pragma region 자막
void FDialogueGraphEditor::SetSubtitleText(const FString& SubtitleText) const
{
    if (Tb_CurrentLine.IsValid())
    {
        Tb_CurrentLine->SetText(FText::FromString(SubtitleText));
        ShowSubtitle();
    }
}

void FDialogueGraphEditor::ShowSubtitle() const
{
    if (CurrentLineOverlay
        && Tb_CurrentLine.IsValid()
        && !Tb_CurrentLine->GetText().IsEmpty())
    {
        CurrentLineOverlay->GetWidget()->SetVisibility(EVisibility::Visible);
    }
}

void FDialogueGraphEditor::HideSubtitle() const
{
    if (CurrentLineOverlay)
    {
        CurrentLineOverlay->GetWidget()->SetVisibility(EVisibility::Hidden);
    }
}

TObjectPtr<UDialogueGraph> FDialogueGraphEditorUtils::GetDialogueAsset(const UDialogueEdGraphNode* Node) const
{
    const auto Asset = Node->GetTypedOuter<UDialogueGraph>();
    checkf(Asset, TEXT("노드 생성과정에서 잘못 Outer를 설정했습니다."));
    return Asset;
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
void FDialogueGraphEditorUtils::OnBlueprintChanged(UBlueprint* Blueprint)
{
    TArray<FString> GraphNames;

    // FunctionGraph에 존재하는 함수 이름만 수집한다.
    Algo::Transform(Blueprint->FunctionGraphs, GraphNames, [](const TObjectPtr<UEdGraph>& Graph)
    {
        return Graph->GetName();
    });

    const auto DialogueGraphAsset = Blueprint->GetTypedOuter<UDialogueGraph>();
    const auto GraphData          = Cast<UDialogueEdGraphAssetData>(DialogueGraphAsset->AssetUserData);
    const auto DialogueGraph      = GraphData->DialogueEdGraph;
    for (TObjectPtr EdGraphNode : DialogueGraph->Nodes)
    {
        DialogueGraph->Modify();
        if (!EdGraphNode)
        {
            continue;
        }

        // 각 함수 이름에 대해 업데이트를 수행한다.
        const auto Node = Cast<UDialogueEdGraphNode>(EdGraphNode.Get());

        if (const auto CanSelectThisItem = Cast<ICanSelectThisNode>(Node))
        {
            if (const auto FunctionName = CanSelectThisItem->GetCanSelectThisNodeFunctionName(); FunctionName != NAME_None)
            {
                const auto Found = Algo::FindByPredicate(GraphNames, [FunctionName = FunctionName.ToString()](const FString& GraphName)
                {
                    return GraphName.Equals(FunctionName);
                });
                // 만약 선택한 경우 활성화 하는 메서드 이름이 위 리스트에 없는 경우
                if (!Found)
                {
                    // 이것을 Name_None으로 설정하여 실행하지 않도록 수정한다.
                    CanSelectThisItem->SetCanSelectThisNodeFunctionName(NAME_None);
                }
            }
        }

        if (const auto WhenSelectThisItem = Cast<IWhenSelectThisNode>(Node))
        {
            if (const auto FunctionName = WhenSelectThisItem->GetWhenSelectThisNodeFunctionName(); FunctionName != NAME_None)
            {
                const auto Found = Algo::FindByPredicate(GraphNames, [FunctionName = FunctionName.ToString()](const FString& GraphName)
                {
                    return GraphName.Equals(FunctionName);
                });
                // 만약 선택한 경우 활성화 하는 메서드 이름이 위 리스트에 없는 경우
                if (!Found)
                {
                    // 이것을 Name_None으로 설정하여 실행하지 않도록 수정한다.
                    WhenSelectThisItem->SetWhenSelectThisNodeFunctionName(NAME_None);
                }
            }
        }
    }
}
#pragma endregion


#undef LOCTEXT_NAMESPACE
