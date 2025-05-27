#include "Character/DialogueCharacterEditor.h"
#include "Editor.h"
#include "Character/DialogueCharacterAsset.h"
#include "Framework/Docking/TabManager.h"
#include "Widgets/Docking/SDockTab.h"

#define LOCTEXT_NAMESPACE "DialogueCharacterEditor"

FDialogueCharacterEditor::FDialogueCharacterEditor()
    : DialogueCharacterAsset(nullptr)
{
    if ( GEditor )
    {
        GEditor->RegisterForUndo(this);
    }
}

FDialogueCharacterEditor::~FDialogueCharacterEditor()
{
    if ( GEditor )
    {
        GEditor->UnregisterForUndo(this);
    }
}

void FDialogueCharacterEditor::InitDialogueCharacterEditor(const EToolkitMode::Type Mode,
                                                           const TSharedPtr<IToolkitHost>& InitToolkitHost,
                                                           UDialogueCharacterAsset* InDialogueCharacterAsset)
{
    // 캐릭터 에셋 설정
    DialogueCharacterAsset = InDialogueCharacterAsset;

    // (명령어 등 필요한 경우 여기에 초기화 코드 추가)

    // 레이아웃 정의 (그래프 에디터와 유사한 구조, 추후 확장 가능)
    const TSharedRef<FTabManager::FLayout> DefaultLayout = FTabManager::NewLayout("DefaultDialogueCharacterEditorLayout")
        ->AddArea(
            FTabManager::NewPrimaryArea()
            ->SetOrientation(Orient_Vertical)
            ->Split(
                FTabManager::NewSplitter()
                ->SetOrientation(Orient_Horizontal)
                ->SetSizeCoefficient(0.9f)
                ->Split(
                    FTabManager::NewStack()
                    ->SetSizeCoefficient(1.0f)
                    ->AddTab(FName("DetailsTab"), ETabState::OpenedTab)
                )
            )
        );

    // 에셋 에디터 초기화
    constexpr bool bCreateDefaultStandaloneMenu = true;
    constexpr bool bCreateDefaultToolbar        = true;
    InitAssetEditor(Mode,
                    InitToolkitHost,
                    FName("DialogueCharacterEditorApp"),
                    DefaultLayout,
                    bCreateDefaultStandaloneMenu,
                    bCreateDefaultToolbar,
                    InDialogueCharacterAsset);
}

// 탭 ID 정의
namespace DialogueCharacterEditorTabs
{
    static const FName DetailsID = TEXT("DialogueCharacterEditor_Details");
}

void FDialogueCharacterEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
    FWorkflowCentricApplication::RegisterTabSpawners(InTabManager);
    // 속성 편집기 탭 등록
    InTabManager->RegisterTabSpawner(DialogueCharacterEditorTabs::DetailsID,
        FOnSpawnTab::CreateSP(this, &FDialogueCharacterEditor::PropertyTabSpawner))
        .SetDisplayName(LOCTEXT("Details", "Detail"))
        .SetGroup(GetWorkspaceMenuCategory());
}

void FDialogueCharacterEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
    FWorkflowCentricApplication::UnregisterTabSpawners(InTabManager);
}

TSharedRef<SDockTab> FDialogueCharacterEditor::PropertyTabSpawner(const FSpawnTabArgs& Args)
{
    // 속성 에디터 생성
    FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
    FDetailsViewArgs DetailsViewArgs;
    DetailsViewArgs.bUpdatesFromSelection = false;
    DetailsViewArgs.bLockable             = false;
    DetailsViewArgs.bAllowSearch          = true;
    DetailsViewArgs.NameAreaSettings      = FDetailsViewArgs::ObjectsUseNameArea;
    DetailsViewArgs.bHideSelectionTip     = false;
    PropertyEditor = PropertyEditorModule.CreateDetailView(DetailsViewArgs);

    // 처음 편집할 데이터
    PropertyEditor->SetObject(DialogueCharacterAsset);

    TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab)
        .Label(LOCTEXT("DetailsPanel", "Details"))
        [
            PropertyEditor.ToSharedRef()
        ];

    return SpawnedTab;
}

FName FDialogueCharacterEditor::GetToolkitFName() const
{
    return FName("DialogueCharacterEditor");
}

FText FDialogueCharacterEditor::GetBaseToolkitName() const
{
    return LOCTEXT("AppLabel", "Dialogue Character Editor");
}

FText FDialogueCharacterEditor::GetToolkitName() const
{
    return LOCTEXT("ToolkitName", "Dialogue Character");
}

FText FDialogueCharacterEditor::GetToolkitToolTipText() const
{
    return LOCTEXT("ToolkitToolTip", "Edit a Dialogue Character Asset");
}

FString FDialogueCharacterEditor::GetWorldCentricTabPrefix() const
{
    return TEXT("DialogueCharacter ");
}

FLinearColor FDialogueCharacterEditor::GetWorldCentricTabColorScale() const
{
    return FLinearColor(0.0f, 0.5f, 1.0f, 0.5f);
}

#undef LOCTEXT_NAMESPACE
