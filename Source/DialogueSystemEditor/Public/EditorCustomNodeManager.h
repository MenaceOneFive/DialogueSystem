// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Graph/Node/DialogueEditorCustomNodeDefinition.h"

DECLARE_DELEGATE_RetVal_OneParam(TSharedPtr<SGraphNode>, FOnMakeWidgetForGraphNode, UEdGraphNode*)

class DIALOGUESYSTEMEDITOR_API FEditorCustomNodeManager
{
public:
    void RegisterGraphNodeCreationDelegate(UClass* NodeType, FOnMakeWidgetForGraphNode Delegate);
    void RegisterCustomNodeDefinition(UClass* Class, const TSharedPtr<FDialogueEditorCustomNodeDefinition>& Definition);
    TArray<TSharedPtr<FDialogueEditorCustomNodeDefinition>> GetAllCustomNodeDefinitions();

    FOnMakeWidgetForGraphNode GetWidgetCreationDelegate(const UClass* NodeType) const;
    FDialogueEditorCustomNodeDefinition* GetCustomNodeDefinition(const UClass* Class) const;

    void UnregisterGraphNodeCreationDelegate(UClass* NodeType);
    void UnregisterCustomNodeDefinition(UClass* Class);

private:
    TMap<UClass*, FOnMakeWidgetForGraphNode> EditorGraphToWidgetDelegate;
    TMap<UClass*, TSharedPtr<FDialogueEditorCustomNodeDefinition>> CustomNodeDefinitions;
};
