// Fill out your copyright notice in the Description page of Project Settings.
#include "Graph/DialogueGraph.h"
#include "Graph/Node/DialogueGraphNode.h"

#if WITH_EDITORONLY_DATA

UBlueprint* UDialogueGraph::GetBlueprintInstance() const
{
    return BlueprintInstance;
}

TObjectPtr<UClass> UDialogueGraph::GetBlueprintClass() const
{
    return BlueprintClass;
}

void UDialogueGraph::SetBlueprintInstance(UBlueprint* InBlueprintInstance)
{
    if ( InBlueprintInstance )
    {
        BlueprintClass    = InBlueprintInstance->GeneratedClass.Get();
        BlueprintInstance = InBlueprintInstance;
    }
    else
    {
        BlueprintClass = nullptr;
    }
}

FString UDialogueGraph::GetDescription() const
{
    return Description;
}

FString UDialogueGraph::GetMemo() const
{
    return Memo;
}

void UDialogueGraph::OnDirectorRecompiled(UBlueprint* InBlueprint)
{
    check(InBlueprint == BlueprintInstance)
    BlueprintClass = InBlueprint->GeneratedClass.Get();
}

FString UDialogueGraph::GetDirectorBlueprintName() const
{
    return GetFName().ToString() + "_DirectorBP";
}
#endif
void UDialogueGraph::BeginDestroy()
{
    UE_LOG(LogMemory, Display, TEXT("UDialogueGraph"))
    Super::BeginDestroy();
}

bool UDialogueGraph::AddNode(UDialogueGraphNode* GraphNode)
{
    if ( ContainsNode(GraphNode) )
    {
        return false;
    }
    Nodes.Add(GraphNode->GetNodeID(), GraphNode);
    return true;
}

UDialogueGraphNode* UDialogueGraph::GetNode(const FGuid NodeID)
{
    if ( !Nodes.Contains(NodeID) )
    {
        return nullptr;
    }
    return Nodes[NodeID];
}

UDialogueStartNode* UDialogueGraph::GetStartNode() const
{
    return StartNode;
}

void UDialogueGraph::SetStartNode(UDialogueStartNode* InStartNode)
{
    StartNode = InStartNode;
}

void UDialogueGraph::Clear()
{
    for ( const auto Pair : Nodes )
    {
        Pair.Value->Rename();
    }
    Nodes.Empty();
    if ( StartNode )
    {
        StartNode->Rename();
        StartNode = nullptr;
    }
}

TArray<const UDialogueGraphNode*> UDialogueGraph::GetNodes() const
{
    TArray<UDialogueGraphNode*> ValueArray;
    Nodes.GenerateValueArray(ValueArray);
    return TArray<const UDialogueGraphNode*>(ValueArray);
}

bool UDialogueGraph::ContainsNode(const UDialogueGraphNode* GraphNode) const
{
    return Nodes.Contains(GraphNode->GetNodeID());
}

const UDialogueGraphNode* UDialogueGraph::GetNode(const FGuid NodeID) const
{
    if ( !Nodes.Contains(NodeID) )
    {
        return nullptr;
    }
    return Nodes[NodeID];
}
