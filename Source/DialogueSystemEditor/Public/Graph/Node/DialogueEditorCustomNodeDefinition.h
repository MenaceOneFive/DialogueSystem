// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueEdGraphCustomNode.h"

class UDialogueCustomNode;

enum class EOutgoingConnection:uint8
{
    One,
    Many
};

class DIALOGUESYSTEMEDITOR_API FDialogueEditorCustomNodeDefinition
{
public:
    virtual ~FDialogueEditorCustomNodeDefinition() = default;

    virtual TSubclassOf<UDialogueEdGraphCustomNode> GetEditorNodeType() const = 0;

    virtual TSubclassOf<UDialogueCustomNode> GetRuntimeNodeType() const = 0;

    virtual void InitializeRuntimeNodeWithEditorNode(UDialogueCustomNode* RuntimeNode, UDialogueEdGraphCustomNode* EditorNode) const = 0;

    virtual TSharedPtr<SGraphNode> MakeSlateWidgetForNode(UDialogueEdGraphCustomNode* EditorNode) const = 0;

    virtual EOutgoingConnection GetConnectionType() = 0;

    virtual FString GetNodeName() const = 0;
    virtual FString GetNodeDescription() const = 0;
    virtual FString GetNodeSearchKeyword() const = 0;
};
