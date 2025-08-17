// Fill out your copyright notice in the Description page of Project Settings.


#include "RuntimeCustomNodeManager.h"
#include "Graph/Node/DialogueSystemCustomNodeDefinition.h"


FRuntimeCustomNodeManager::FRuntimeCustomNodeManager()
{
}

void FRuntimeCustomNodeManager::RegisterCustomNodeDefinition(const TSharedPtr<FDialogueSystemCustomNodeDefinition>& Definition)
{
    CustomNodeDefinitions.Emplace(Definition->GetRuntimeCustomNodeType().Get(), Definition);
}

void FRuntimeCustomNodeManager::UnregisterCustomNodeDefinition(UClass* Class)
{
    CustomNodeDefinitions.Remove(Class);
}

FDialogueSystemCustomNodeDefinition* FRuntimeCustomNodeManager::GetDefinitionOfCustomNode(UClass* Class) const
{
    if (CustomNodeDefinitions.Contains(Class))
    {
        return CustomNodeDefinitions[Class].Get();
    }
    return nullptr;
}
