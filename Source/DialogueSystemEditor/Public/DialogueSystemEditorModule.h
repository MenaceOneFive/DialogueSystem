// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "EditorCustomNodeManager.h"
#include "Character/DialogueCharacterAsset.h"
#include "Graph/Node/DialogueCustomNode.h"
#include "Graph/Node/DialogueEdGraphCustomNode.h"
#include "Graph/Node/DialogueEditorCustomNodeDefinition.h"
#include "Modules/ModuleManager.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"
#include "DialogueSystemEditorModule.generated.h"

class FDialogueEditorCustomNodeDefinition;
class FDialogueGraphPanelNodeFactory;
class FDialogueGraphEditor;
class FAssetTypeActions_DialogueGraph;


UCLASS()
class DIALOGUESYSTEMEDITOR_API UDialogueEdGraphQTENode : public UDialogueEdGraphCustomNode
{
    GENERATED_BODY()

public:
    virtual FText GetDefaultSelectionName() const override;
    UPROPERTY(EditAnywhere)
    TSoftObjectPtr<ULevelSequence> LevelSequenceToPlay;

    UPROPERTY(EditAnywhere)
    FString Name;
};

struct DIALOGUESYSTEMEDITOR_API FDialogueEdQTENodeDefinition : public FDialogueEditorCustomNodeDefinition
{
    virtual TSubclassOf<UDialogueEdGraphCustomNode> GetEditorNodeType() const override;
    virtual TSubclassOf<UDialogueCustomNode> GetRuntimeNodeType() const override;
    virtual void InitializeRuntimeNodeWithEditorNode(UDialogueCustomNode* RuntimeNode, UDialogueEdGraphCustomNode* EditorNode) const override;
    virtual TSharedPtr<SGraphNode> MakeSlateWidgetForNode(UDialogueEdGraphCustomNode* EditorNode) const override;
    virtual EOutgoingConnection GetConnectionType() override;

    virtual FString GetNodeName() const override
    {
        return "QTENode";
    };

    virtual FString GetNodeDescription() const override
    {
        return "QTENode";
    };

    virtual FString GetNodeSearchKeyword() const override
    {
        return "QTENode";
    };
};

class DIALOGUESYSTEMEDITOR_API FDialogueSystemEditorModule final : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    FEditorCustomNodeManager* GetCustomNodeManager() const;

private:
    void RegisterDefaultNodeWidgetCreationDelegates() const;
    void UnregisterDefaultNodeWidgetCreationDelegates() const;

private:
    // 시퀀서 통합을 위한 델리게이트 핸들
    FDelegateHandle DialogueTrackEditorDelegateHandle;
    FDelegateHandle DialogueLineTrackEditorDelegateHandle;
    FDelegateHandle StoryTrackEditorDelegateHandle;
    FDelegateHandle PlayerControlTrackEditorDelegateHandle;

    TSharedPtr<FDocumentTracker> DocumentManager;
    TSharedPtr<FDialogueGraphPanelNodeFactory> GraphNodeFactory;

    TSharedPtr<FAssetTypeActions_DialogueGraph> GraphAssetTypeActions;
    TSharedPtr<FAssetTypeActions_DialogueCharacter> CharacterAssetTypeActions;
    TUniquePtr<FEditorCustomNodeManager> CustomNodeManager;
};
