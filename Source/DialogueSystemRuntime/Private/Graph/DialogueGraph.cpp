// Fill out your copyright notice in the Description page of Project Settings.
#include "Graph/DialogueGraph.h"
#include "Graph/Node/DialogueGraphNode.h"
#include "Graph/Node/DialogueStartNode.h"

TObjectPtr<UBlueprint> UDialogueGraph::GetBlueprintInstance() const
{
    return BlueprintInstance;
}

TObjectPtr<UClass> UDialogueGraph::GetBlueprintClass() const
{
    return BlueprintClass;
}

FString UDialogueGraph::GetDirectorBlueprintName() const
{
    return GetFName().ToString() + "_DirectorBP";
}

#if WITH_EDITOR
void UDialogueGraph::SetBlueprintInstance(UBlueprint* InBlueprintInstance)
{
    if (InBlueprintInstance)
    {
        BlueprintClass    = InBlueprintInstance->GeneratedClass.Get();
        BlueprintInstance = InBlueprintInstance;
    }
    else
    {
        BlueprintClass = nullptr;
    }
}
#endif

#if WITH_EDITORONLY_DATA
FString UDialogueGraph::GetDescription() const
{
    return Description;
}

FString UDialogueGraph::GetMemo() const
{
    return Memo;
}

void UDialogueGraph::OnDirectorRecompiled(TObjectPtr<UBlueprint> InBlueprint)
{
    check(InBlueprint == BlueprintInstance)
    BlueprintClass = InBlueprint->GeneratedClass.Get();
}

#endif
void UDialogueGraph::BeginDestroy()
{
    UE_LOG(LogMemory, Display, TEXT("UDialogueGraph"))
    Super::BeginDestroy();
}


UDialogueGraphNode* UDialogueGraph::GetNode(const FGuid NodeID)
{
    if (!Nodes.Contains(NodeID))
    {
        return nullptr;
    }
    return Nodes[NodeID];
}

UDialogueStartNode* UDialogueGraph::GetStartNode() const
{
    return StartNode;
}
#if WITH_EDITOR
bool UDialogueGraph::AddNode(UDialogueGraphNode* GraphNode)
{
    if (ContainsNode(GraphNode))
    {
        return false;
    }
    Nodes.Add(GraphNode->GetNodeID(), GraphNode);
    return true;
}

void UDialogueGraph::SetStartNode(TObjectPtr<UDialogueStartNode> InStartNode)
{
    StartNode = InStartNode;
}

void UDialogueGraph::Clear()
{
    for (const auto Pair : Nodes)
    {
        if (Pair.Value)
        {
            Pair.Value->Rename(nullptr, nullptr);
        }
    }
    Nodes.Empty();
    if (StartNode)
    {
        StartNode->Rename(nullptr, nullptr);
        StartNode = nullptr;
    }
}
#endif

TArray<TObjectPtr<const UDialogueGraphNode>> UDialogueGraph::GetNodes() const
{
    TArray<UDialogueGraphNode*> ValueArray;
    Nodes.GenerateValueArray(ValueArray);
    return TArray<TObjectPtr<const UDialogueGraphNode>>(ValueArray);
}

bool UDialogueGraph::ContainsNode(const UDialogueGraphNode* GraphNode) const
{
    return Nodes.Contains(GraphNode->GetNodeID());
}

TObjectPtr<const UDialogueGraphNode> UDialogueGraph::GetNode(const FGuid NodeID) const
{
    if (!Nodes.Contains(NodeID))
    {
        return nullptr;
    }
    return Nodes[NodeID];
}
