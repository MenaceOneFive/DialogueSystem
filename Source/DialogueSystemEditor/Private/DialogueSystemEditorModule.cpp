#include "DialogueSystemEditorModule.h"

#include "Graph/DialogueGraphAssetFactory.h"
#include "Graph/DialogueGraphEditorCommands.h"
#include "Graph/DialogueGraphPanelNodeFactory.h"
#include "MovieScene/TrackEditor/DialogueLineTrackEditor.h"
#include "MovieScene/TrackEditor/DialogueTrackEditor.h"

void FDialogueSystemEditorModule::StartupModule()
{
    // 시퀀서에 트랙 에디터 등록
    ISequencerModule& SequencerModule     = FModuleManager::LoadModuleChecked<ISequencerModule>("Sequencer");
    DialogueLineTrackEditorDelegateHandle = SequencerModule.RegisterTrackEditor(FOnCreateTrackEditor::CreateStatic(&FDialogueLineTrackEditor::CreateTrackEditor));
    DialogueTrackEditorDelegateHandle     = SequencerModule.RegisterTrackEditor(FOnCreateTrackEditor::CreateStatic(&FDialogueTrackEditor::CreateTrackEditor));

    // TODO: 모델 정의
    // SequencerModule.RegisterTrackModel(FOnCreateTrackModel::CreateStatic());

    IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
    AssetTools.RegisterAdvancedAssetCategory(DialogueAsset::CategoryBase, FText::FromString("Dialogue System"));
    GraphAssetTypeActions = MakeShareable(new FAssetTypeActions_DialogueGraph());
    AssetTools.RegisterAssetTypeActions(GraphAssetTypeActions.ToSharedRef());

    GraphNodeFactory = MakeShared<FDialogueGraphPanelNodeFactory>();
    FEdGraphUtilities::RegisterVisualNodeFactory(GraphNodeFactory);

    // DialogueCharacterAsset 등록
    CharacterAssetTypeActions = MakeShared<FAssetTypeActions_DialogueCharacter>();
    AssetTools.RegisterAssetTypeActions(CharacterAssetTypeActions.ToSharedRef());

    // 에디터 커멘드 등록
    {
        FDialogueGraphEditorCommands::Register();
    }

    //썸네일 등록 : 캐릭터 에셋의 경우 캐릭터의 프로필 이미지가 에셋의 썸네일이 되도록 해야 한다.
    {
        UThumbnailManager::Get().RegisterCustomRenderer(UDialogueCharacterAsset::StaticClass(), UDialogueCharacterThumbnailRenderer::StaticClass());
    }
}

void FDialogueSystemEditorModule::ShutdownModule()
{
    if ( FModuleManager::Get().IsModuleLoaded("Sequencer") )
    {
        ISequencerModule& SequencerModule = FModuleManager::LoadModuleChecked<ISequencerModule>("Sequencer");
        SequencerModule.UnRegisterTrackEditor(DialogueLineTrackEditorDelegateHandle);
        SequencerModule.UnRegisterTrackEditor(DialogueTrackEditorDelegateHandle);
        SequencerModule.UnRegisterTrackEditor(StoryTrackEditorDelegateHandle);
        SequencerModule.UnRegisterTrackEditor(PlayerControlTrackEditorDelegateHandle);
    }
    if ( FModuleManager::Get().IsModuleLoaded("AssetTools") )
    {
        IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
        AssetTools.UnregisterAssetTypeActions(GraphAssetTypeActions.ToSharedRef());
        AssetTools.UnregisterAssetTypeActions(CharacterAssetTypeActions.ToSharedRef());
    }

    if ( FModuleManager::Get().IsModuleLoaded("PropertyEditor") )
    {
        FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
        PropertyModule.UnregisterCustomClassLayout(UDialogueEdGraphDialogueLineNode::StaticClass()->GetFName());
        PropertyModule.UnregisterCustomPropertyTypeLayout(FName("FDialogueLineCreationPrompt"));
    }

    // 에디터 커멘드 등록 해제
    {
        FDialogueGraphEditorCommands::Unregister();
    }

    // 썸네일 등록 해제
    if ( FModuleManager::Get().IsModuleLoaded("ThumbnailManager") )
    {
        UThumbnailManager::Get().UnregisterCustomRenderer(UDialogueCharacterAsset::StaticClass());
    }

    FEdGraphUtilities::UnregisterVisualNodeFactory(GraphNodeFactory);
}

IMPLEMENT_MODULE(FDialogueSystemEditorModule, DialogueSystemEditor)
