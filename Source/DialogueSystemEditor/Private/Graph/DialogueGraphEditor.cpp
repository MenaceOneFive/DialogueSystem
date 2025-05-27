#include "Graph/DialogueGraphEditor.h"

#include "GraphEditor.h"
#include "PropertyEditorModule.h"
#include "EdGraph/EdGraph.h"
#include "EdGraph/EdGraphNode.h"
#include "Framework/Commands/GenericCommands.h"
#include "Framework/Docking/TabManager.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Graph/DialogueEdGraphNodes.h"
#include "Graph/DialogueGraphAsset.h"
#include "Graph/DialogueGraphEditorCommands.h"
#include "Graph/Slate/PropertyEditor/DialogueLineCreationPromptDetail.h"
#include "Graph/Slate/PropertyEditor/GraphEditorDetailCustomization.h"
#include "Modules/ModuleManager.h"
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
}

void FDialogueGraphEditor::InitDialogueGraphEditor(const EToolkitMode::Type Mode,
                                                   const TSharedPtr<IToolkitHost>& InitToolkitHost,
                                                   UDialogueGraphAsset* InDialogueGraph)
{
    // 그래프 에셋 설정
    DialogueGraphAsset = InDialogueGraph;

    // 그래프 에디터 명령어 초기화
    if ( !GraphEditorCommands.IsValid() )
    {
        GraphEditorCommands = MakeShareable(new FUICommandList);

        // 여기에 그래프 에디터 명령어 매핑 추가
        GraphEditorCommands->MapAction(FDialogueGraphEditorCommands::Get().Command1,
                                       FExecuteAction::CreateSP(this, &FDialogueGraphEditor::OnCreateComment),
                                       FCanExecuteAction::CreateLambda([]() { return true; }));

        // Delete가 눌린 경우 -> 노드를 삭제함
        GraphEditorCommands->MapAction(FGenericCommands::Get().Delete,
                                       FExecuteAction::CreateSP(this, &FDialogueGraphEditor::OnDeleteKeyPressed));
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
                             ->AddTab(FName("YourWidgetTabID"), ETabState::OpenedTab))
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
    UDialogueEdGraph* DialogueEdGraph = Cast<UDialogueEdGraph>(DialogueGraphAsset->DialogueGraph);
    check(DialogueEdGraph) // 가능한 경우 : DialogueGraphAsset에서 DialogueGraph가 적절히 초기화되지 않았음
    TSharedRef<SGraphEditor> GraphWidget = CreateGraphEditorWidget(DialogueEdGraph);
    return SNew(SDockTab)
        .TabRole(PanelTab)
        [
            GraphWidget
        ];
}

TSharedRef<SDockTab> FDialogueGraphEditor::PropertyTabSpawner(const FSpawnTabArgs& Args)
{
    // 속성 에디터 생성
    FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
    FDetailsViewArgs DetailsViewArgs;
    DetailsViewArgs.bUpdatesFromSelection = false;
    DetailsViewArgs.bLockable             = false;
    DetailsViewArgs.bAllowSearch          = true;
    DetailsViewArgs.NameAreaSettings      = FDetailsViewArgs::ObjectsUseNameArea;
    DetailsViewArgs.bHideSelectionTip     = false;
    PropertyEditor                        = PropertyEditorModule.CreateDetailView(DetailsViewArgs);

#pragma region DetailTypeCustomization // DetailTypeCustomization를 등록
    
    PropertyEditor->RegisterInstancedCustomPropertyTypeLayout(FDialogueLineCreationPrompt::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FDialogueLineCreationPromptDetailTypeCustomization::MakeInstance));
    PropertyEditor->RegisterInstancedCustomPropertyLayout(UDialogueEdGraphDialogueLineNode::StaticClass(), FOnGetDetailCustomizationInstance::CreateStatic(&FDialogueEdDialogueLineNodeDetail::MakeInstance));
    
#pragma endregion

    // 처음 편집할 데이터
    PropertyEditor->SetObject(DialogueGraphAsset);

    TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab)
        .Label(LOCTEXT("DetailsPanel", "Details"))
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
    if ( bSuccess )
    {
        // 그래프 에디터 위젯 새로고침
        if ( FocusedGraphEdPtr.IsValid() )
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
    if ( bSuccess )
    {
        // 그래프 에디터 위젯 새로고침
        if ( FocusedGraphEdPtr.IsValid() )
        {
            FocusedGraphEdPtr.Pin()->NotifyGraphChanged();
        }

        // 속성 에디터 새로고침
        PropertyEditor->ForceRefresh();
    }
}

#pragma endregion

/// <summary>
/// 에디터 윈도우 이벤트 리스너 (Delete 키 이벤트 등)
/// </summary>
#pragma region 에디터_윈도우_이벤트_리스너

void FDialogueGraphEditor::OnDeleteKeyPressed()
{
    const FScopedTransaction Transaction(LOCTEXT("DeleteNode", "Delete node from graph"));
    if ( const TSharedPtr<SGraphEditor>
        GraphEditor = FocusedGraphEdPtr.Pin() )
    {
        UEdGraph* FocusedGraph = GraphEditor->GetCurrentGraph();
        for ( UObject* SelectedNode : GraphEditor->GetSelectedNodes() )
        {
            if ( SelectedNode->IsA<UDialogueEdGraphStartNode>() )
            {
                FNotificationInfo Info(LOCTEXT("CannotRemoveStartNode", "시작노드는 삭제할 수 없습니다."));
                Info.ExpireDuration = 2.0f;
                FSlateNotificationManager::Get().AddNotification(Info);
                continue; // 어떠한 경우에도 시작노드는 삭제할 수 없다.
            }
            UDialogueEdGraphNode* NodeToRemove = Cast<UDialogueEdGraphNode>(SelectedNode);
            FocusedGraph->RemoveNode(NodeToRemove);
        }
    }
}

#pragma endregion

/// <summary>
/// 그래프 에디터 위젯 이벤트 리스너 (노드 선택 변경, 더블 클릭 등)
/// </summary>
#pragma region 그래프_에디터_위젯_이벤트_리스너

void FDialogueGraphEditor::SetupGraphEditorEvents(UDialogueEdGraph* InGraph,
                                                  SGraphEditor::FGraphEditorEvents& InEvents)
{
    // 노드 선택 변경 이벤트 설정
    InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FDialogueGraphEditor::OnSelectedNodesChanged);
    InEvents.OnDoubleClicked    = SGraphEditor::FOnDoubleClicked::CreateSP(this, &FDialogueGraphEditor::OnDoubleClicked);
}

void FDialogueGraphEditor::OnSelectedNodesChanged(const TSet<UObject*>& NewSelection)
{
    // 선택된 노드에 따라 속성 에디터 업데이트
    if ( NewSelection.Num() == 1 )
    {
        for ( UObject* SelectionObj : NewSelection )
        {
            check(SelectionObj->IsA<UDialogueEdGraphNode>())
            if ( SelectionObj )
            {
                PropertyEditor->SetObject(SelectionObj);
                return;
            }
        }
    }

    // 선택된 노드가 없거나 여러 개인 경우 대화 그래프 에셋 표시
    PropertyEditor->SetObject(DialogueGraphAsset);
}

void FDialogueGraphEditor::OnDoubleClicked()
{
}

void FDialogueGraphEditor::OnCreateComment()
{
    // 코멘트 노드 생성 로직
    // ...
}

#pragma endregion

/// <summary>
/// 그래프 에디터 위젯 팩토리 메서드
/// </summary>
#pragma region 위젯_팩토리_메서드

TSharedRef<SGraphEditor> FDialogueGraphEditor::CreateGraphEditorWidget(UDialogueEdGraph* InGraph)
{
    // 그래프 에디터 이벤트 설정
    SGraphEditor::FGraphEditorEvents InEvents;
    SetupGraphEditorEvents(InGraph, InEvents);

    // 그래프 에디터 위젯 생성
    TSharedRef<SGraphEditor> GraphEditor = SNew(SGraphEditor)
        .AdditionalCommands(GraphEditorCommands)
        .GraphToEdit(InGraph)
        .GraphEvents(InEvents);

    // 포커스된 그래프 에디터 설정
    FocusedGraphEdPtr = GraphEditor;

    return GraphEditor;
}

#pragma endregion

#undef LOCTEXT_NAMESPACE
