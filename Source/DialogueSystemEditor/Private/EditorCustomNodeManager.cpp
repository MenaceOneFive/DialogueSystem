// Fill out your copyright notice in the Description page of Project Settings.

#include "EditorCustomNodeManager.h"

FDialogueEditorCustomNodeDefinition* FEditorCustomNodeManager::GetCustomNodeDefinition(const UClass* Class) const
{
    if (CustomNodeDefinitions.Contains(Class))
    {
        return CustomNodeDefinitions[Class].Get();
    }
    return nullptr;
}

FOnMakeWidgetForGraphNode FEditorCustomNodeManager::GetWidgetCreationDelegate(const UClass* NodeType) const
{
    if (!EditorGraphToWidgetDelegate.Contains(NodeType))
    {
        return nullptr;
    }
    return EditorGraphToWidgetDelegate[NodeType];
}

TArray<TSharedPtr<FDialogueEditorCustomNodeDefinition>> FEditorCustomNodeManager::GetAllCustomNodeDefinitions()
{
    TArray<TSharedPtr<FDialogueEditorCustomNodeDefinition>> ValueArray;
    CustomNodeDefinitions.GenerateValueArray(ValueArray);
    return ValueArray;
}

void FEditorCustomNodeManager::UnregisterGraphNodeCreationDelegate(UClass* NodeType)
{
    if (EditorGraphToWidgetDelegate.Contains(NodeType))
    {
        EditorGraphToWidgetDelegate.Remove(NodeType);
    }
}

void FEditorCustomNodeManager::RegisterGraphNodeCreationDelegate(UClass* NodeType, FOnMakeWidgetForGraphNode Delegate)
{
    EditorGraphToWidgetDelegate.Emplace(NodeType, Delegate);
}

void FEditorCustomNodeManager::RegisterCustomNodeDefinition(UClass* Class, const TSharedPtr<FDialogueEditorCustomNodeDefinition>& Definition)
{
    CustomNodeDefinitions.Emplace(Class, Definition);
    RegisterGraphNodeCreationDelegate(Class, FOnMakeWidgetForGraphNode::CreateLambda([this, Class](UEdGraphNode* GraphNode)
    {
        const auto CustomNodeDefinition = this->GetCustomNodeDefinition(Class);
        const auto Node                 = Cast<UDialogueEdGraphCustomNode>(GraphNode);
        // if (CustomNodeDefinition && Node)
        // {
        return CustomNodeDefinition->MakeSlateWidgetForNode(Node);
        // }
        // return nullptr;
    }));
}

void FEditorCustomNodeManager::UnregisterCustomNodeDefinition(UClass* Class)
{
    if (CustomNodeDefinitions.Contains(Class))
    {
        CustomNodeDefinitions.Remove(Class);
        UnregisterGraphNodeCreationDelegate(Class);
    }
}
