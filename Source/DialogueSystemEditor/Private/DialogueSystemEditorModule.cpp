#include "DialogueSystemEditorModule.h"

#include "DialogueSystemRuntimeModule.h"
#include "EdGraphNode_Comment.h"
#include "EditorCustomNodeManager.h"
#include "ISequencerModule.h"
#include "SGraphNodeComment.h"
#include "SGraphNodeKnot.h"
#include "Graph/DialogueGraphAssetFactory.h"
#include "Graph/DialogueGraphEditorCommands.h"
#include "Graph/DialogueGraphPanelNodeFactory.h"
#include "Graph/Node/DialogueEdGraphCustomNode.h"
#include "Graph/Node/DialogueEdGraphDialogueLineNode.h"
#include "Graph/Node/DialogueEdGraphEndNode.h"
#include "Graph/Node/DialogueEdGraphKnotNode.h"
#include "Graph/Node/DialogueEdGraphSceneNode.h"
#include "Graph/Node/DialogueEdGraphSelectNode.h"
#include "Graph/Node/DialogueEdGraphStartNode.h"
#include "Graph/Node/DialogueEditorCustomNodeDefinition.h"
#include "Graph/Slate/Node/SDialogueEdGraphEndNode.h"
#include "Graph/Slate/Node/SDialogueEdGraphLineNode.h"
#include "Graph/Slate/Node/SDialogueEdGraphSceneNode.h"
#include "Graph/Slate/Node/SDialogueEdGraphSelectNode.h"
#include "Graph/Slate/Node/SDialogueEdGraphStartNode.h"
#include "MovieScene/TrackEditor/DialogueLineTrackEditor.h"

FText UDialogueEdGraphQTENode::GetDefaultSelectionName() const
{
    return NSLOCTEXT("DialoguePlayer", "QTENode", "QTE 노드");
}

TSubclassOf<UDialogueEdGraphCustomNode> FDialogueEdQTENodeDefinition::GetEditorNodeType() const
{
    return UDialogueEdGraphQTENode::StaticClass();
}

TSubclassOf<UDialogueCustomNode> FDialogueEdQTENodeDefinition::GetRuntimeNodeType() const
{
    return UDialogueQTENode::StaticClass();
}

void FDialogueEdQTENodeDefinition::InitializeRuntimeNodeWithEditorNode(UDialogueCustomNode* RuntimeNode, UDialogueEdGraphCustomNode* EditorNode) const
{
    const auto RuntimeQTENode           = Cast<UDialogueQTENode>(RuntimeNode);
    const auto EditorQTENode            = Cast<UDialogueEdGraphQTENode>(EditorNode);
    RuntimeQTENode->LevelSequenceToPlay = EditorQTENode->LevelSequenceToPlay;
    RuntimeQTENode->Name                = EditorQTENode->Name;
}

TSharedPtr<SGraphNode> FDialogueEdQTENodeDefinition::MakeSlateWidgetForNode(UDialogueEdGraphCustomNode* EditorNode) const
{
    return nullptr;
}

EOutgoingConnection FDialogueEdQTENodeDefinition::GetConnectionType()
{
    return EOutgoingConnection::One;
}

void FDialogueSystemEditorModule::StartupModule()
{
    // 시퀀서에 트랙 에디터 등록
    ISequencerModule& SequencerModule     = FModuleManager::LoadModuleChecked<ISequencerModule>("Sequencer");
    DialogueLineTrackEditorDelegateHandle = SequencerModule.RegisterTrackEditor(FOnCreateTrackEditor::CreateStatic(&FDialogueLineTrackEditor::CreateTrackEditor));

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

    CustomNodeManager = MakeUnique<FEditorCustomNodeManager>();
    // 기본 델리게이트 등록
    RegisterDefaultNodeWidgetCreationDelegates();

    // TODO : 모듈 밖으로 이동
    CustomNodeManager->RegisterCustomNodeDefinition(UDialogueEdGraphQTENode::StaticClass(), MakeShared<FDialogueEdQTENodeDefinition>());
}

void FDialogueSystemEditorModule::ShutdownModule()
{
    // TODO : 모듈 밖으로 이동
    CustomNodeManager->UnregisterCustomNodeDefinition(UDialogueEdGraphQTENode::StaticClass());

    if (FModuleManager::Get().IsModuleLoaded("Sequencer"))
    {
        ISequencerModule& SequencerModule = FModuleManager::LoadModuleChecked<ISequencerModule>("Sequencer");
        SequencerModule.UnRegisterTrackEditor(DialogueLineTrackEditorDelegateHandle);
        SequencerModule.UnRegisterTrackEditor(DialogueTrackEditorDelegateHandle);
        SequencerModule.UnRegisterTrackEditor(StoryTrackEditorDelegateHandle);
        SequencerModule.UnRegisterTrackEditor(PlayerControlTrackEditorDelegateHandle);
    }
    if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
    {
        IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
        AssetTools.UnregisterAssetTypeActions(GraphAssetTypeActions.ToSharedRef());
        AssetTools.UnregisterAssetTypeActions(CharacterAssetTypeActions.ToSharedRef());
    }

    if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
    {
        FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
        PropertyModule.UnregisterCustomPropertyTypeLayout(FName("FDialogueLineCreationPrompt"));
    }

    // 에디터 커멘드 등록 해제
    {
        FDialogueGraphEditorCommands::Unregister();
    }

    // 썸네일 등록 해제
    if (FModuleManager::Get().IsModuleLoaded("ThumbnailManager"))
    {
        UThumbnailManager::Get().UnregisterCustomRenderer(UDialogueCharacterAsset::StaticClass());
    }

    FEdGraphUtilities::UnregisterVisualNodeFactory(GraphNodeFactory);
}

FEditorCustomNodeManager* FDialogueSystemEditorModule::GetCustomNodeManager() const
{
    return CustomNodeManager.Get();
}

void FDialogueSystemEditorModule::RegisterDefaultNodeWidgetCreationDelegates() const
{
    CustomNodeManager->RegisterGraphNodeCreationDelegate(UDialogueEdGraphStartNode::StaticClass(), FOnMakeWidgetForGraphNode::CreateLambda([](UEdGraphNode* Node)
    {
        return SNew(SDialogueEdGraphStartNode, Node);
    }));
    CustomNodeManager->RegisterGraphNodeCreationDelegate(UDialogueEdGraphEndNode::StaticClass(), FOnMakeWidgetForGraphNode::CreateLambda([](UEdGraphNode* Node)
    {
        return SNew(SDialogueEdGraphEndNode, Node);
    }));
    CustomNodeManager->RegisterGraphNodeCreationDelegate(UDialogueEdGraphDialogueLineNode::StaticClass(), FOnMakeWidgetForGraphNode::CreateLambda([](UEdGraphNode* Node)
    {
        return SNew(SDialogueEdGraphLineNode, Node);
    }));
    CustomNodeManager->RegisterGraphNodeCreationDelegate(UDialogueEdGraphSceneNode::StaticClass(), FOnMakeWidgetForGraphNode::CreateLambda([](UEdGraphNode* Node)
    {
        return SNew(SDialogueEdGraphSceneNode, Node);
    }));
    CustomNodeManager->RegisterGraphNodeCreationDelegate(UDialogueEdGraphSelectNode::StaticClass(), FOnMakeWidgetForGraphNode::CreateLambda([](UEdGraphNode* Node)
    {
        return SNew(SDialogueEdGraphSelectNode, Node);
    }));
    CustomNodeManager->RegisterGraphNodeCreationDelegate(UDialogueEdGraphKnotNode::StaticClass(), FOnMakeWidgetForGraphNode::CreateLambda([](UEdGraphNode* Node)
    {
        return SNew(SGraphNodeKnot, Node);
    }));
    CustomNodeManager->RegisterGraphNodeCreationDelegate(UEdGraphNode_Comment::StaticClass(), FOnMakeWidgetForGraphNode::CreateLambda([](UEdGraphNode* Node)
    {
        return SNew(SGraphNodeComment, Cast<UEdGraphNode_Comment>(Node));
    }));
}

void FDialogueSystemEditorModule::UnregisterDefaultNodeWidgetCreationDelegates() const
{
    CustomNodeManager->UnregisterGraphNodeCreationDelegate(UDialogueEdGraphStartNode::StaticClass());
    CustomNodeManager->UnregisterGraphNodeCreationDelegate(UDialogueEdGraphEndNode::StaticClass());
    CustomNodeManager->UnregisterGraphNodeCreationDelegate(UDialogueEdGraphDialogueLineNode::StaticClass());
    CustomNodeManager->UnregisterGraphNodeCreationDelegate(UDialogueEdGraphSceneNode::StaticClass());
    CustomNodeManager->UnregisterGraphNodeCreationDelegate(UDialogueEdGraphSelectNode::StaticClass());
    CustomNodeManager->UnregisterGraphNodeCreationDelegate(UDialogueEdGraphKnotNode::StaticClass());
    CustomNodeManager->UnregisterGraphNodeCreationDelegate(UEdGraphNode_Comment::StaticClass());
}

IMPLEMENT_MODULE(FDialogueSystemEditorModule, DialogueSystemEditor)
