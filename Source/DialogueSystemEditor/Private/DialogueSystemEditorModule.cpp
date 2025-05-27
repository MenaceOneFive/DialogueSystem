#include "DialogueSystemEditorModule.h"
#include "AssetToolsModule.h"
#include "ISequencerModule.h"
#include "../Public/MovieScene/TrackEditor/DialogueTrackEditor.h"
#include "../Public/MovieScene/TrackEditor/PlayerControlTrackEditor.h"
#include "../Public/MovieScene/TrackEditor/StoryTrackEditor.h"
#include "Character/DialogueCharacterAsset.h"
#include "Graph/DialogueEdGraphNodes.h"
#include "Graph/DialogueGraphAsset.h"
#include "Graph/DialogueGraphEditorCommands.h"
#include "Graph/DialogueGraphPanelNodeFactory.h"
#include "Graph/Slate/PropertyEditor/DialogueGraphDetailCustomization.h"
#include "Graph/Slate/PropertyEditor/DialogueLineCreationPromptDetail.h"
#include "Graph/Slate/PropertyEditor/GraphEditorDetailCustomization.h"


void FDialogueSystemEditorModule::StartupModule()
{
    // 시퀀서에 트랙 에디터 등록
    ISequencerModule& SequencerModule      = FModuleManager::LoadModuleChecked<ISequencerModule>("Sequencer");
    DialogueTrackEditorDelegateHandle      = SequencerModule.RegisterTrackEditor(FOnCreateTrackEditor::CreateStatic(&FDialogueTrackEditor::CreateTrackEditor));
    StoryTrackEditorDelegateHandle         = SequencerModule.RegisterTrackEditor(FOnCreateTrackEditor::CreateStatic(&FStoryTrackEditor::CreateTrackEditor));
    PlayerControlTrackEditorDelegateHandle = SequencerModule.RegisterTrackEditor(FOnCreateTrackEditor::CreateStatic(&FPlayerControlTrackEditor::CreateTrackEditor));

    // TODO: 모델 정의
    // SequencerModule.RegisterTrackModel(FOnCreateTrackModel::CreateStatic());

    IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
    AssetTools.RegisterAdvancedAssetCategory(DialogueAsset::CategoryBase, FText::FromString("Dialogue System"));
    GraphAssetTypeActions = MakeShareable(new FAssetTypeActions_DialogueGraph());
    AssetTools.RegisterAssetTypeActions(GraphAssetTypeActions.ToSharedRef());

    GraphNodeFactory = MakeShared<FDialogueGraphPanelNodeFactory>();
    FEdGraphUtilities::RegisterVisualNodeFactory(GraphNodeFactory);

    // 프로퍼티 에디터 디테일 등록
    FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    PropertyModule.RegisterCustomClassLayout(UDialogueGraphAsset::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FDialogueGraphDetailCustomization::MakeInstance));
    PropertyModule.RegisterCustomClassLayout(UDialogueEdGraphDialogueLineNode::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FDialogueEdDialogueLineNodeDetail::MakeInstance));
    PropertyModule.RegisterCustomPropertyTypeLayout(FName("FDialogueLineCreationPrompt"), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FDialogueLineCreationPromptDetailTypeCustomization::MakeInstance));


    // DialogueCharacterAsset 등록
    CharacterAssetTypeActions = MakeShared<FAssetTypeActions_DialogueCharacter>();
    AssetTools.RegisterAssetTypeActions(CharacterAssetTypeActions.ToSharedRef());

    FDialogueGraphEditorCommands::Register();
}

void FDialogueSystemEditorModule::ShutdownModule()
{
    if ( FModuleManager::Get().IsModuleLoaded("Sequencer") )
    {
        ISequencerModule& SequencerModule = FModuleManager::LoadModuleChecked<ISequencerModule>("Sequencer");
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

    FDialogueGraphEditorCommands::Unregister();
    FEdGraphUtilities::UnregisterVisualNodeFactory(GraphNodeFactory);
}

IMPLEMENT_MODULE(FDialogueSystemEditorModule, DialogueSystemEditor)
