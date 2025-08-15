// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "ISequencerModule.h"
#include "Character/DialogueCharacterAsset.h"
#include "Modules/ModuleManager.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"

class FDialogueGraphPanelNodeFactory;
class FDialogueGraphEditor;
class FAssetTypeActions_DialogueGraph;

DECLARE_DELEGATE_RetVal_OneParam(TSharedRef<SGraphNode>, FOnMakeWidgetForGraphNode, UEdGraphNode*)

class DIALOGUESYSTEMEDITOR_API FDialogueSystemEditorModule final : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    void RegisterGraphNodeCreationDelegate(UClass* NodeType, FOnMakeWidgetForGraphNode Delegate);

    FOnMakeWidgetForGraphNode GetWidgetCreationDelegate(const UClass* NodeType);

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


    TMap<UClass*, FOnMakeWidgetForGraphNode> EditorGraphToWidgetDelegate;

    void RegisterDefaultNodeWidgetCreationDelegates();
};
