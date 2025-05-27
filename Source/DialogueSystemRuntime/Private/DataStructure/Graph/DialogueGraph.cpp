// Fill out your copyright notice in the Description page of Project Settings.

#include "DataStructure/Graph/DialogueGraph.h"


void UDialogueGraph::Initialize()
{
    if (!bIsInitialized)
    {
        StartNode = NewObject<UDialogueStartNode>(this);
        StartNode->SetNodeID(FGuid::NewGuid());
        Nodes.Add(StartNode->GetNodeID(), StartNode);
        bIsInitialized = true;
    }
}

#if WITH_EDITORONLY_DATA //런타임에는 이 그래프는 수정이 불가능해야 함
bool UDialogueGraph::AddNode(UDialogueGraphNode* GraphNode)
{
    if (ContainsNode(GraphNode))
    {
        return false;
    }
    Nodes.Add(GraphNode->GetNodeID(), GraphNode);
    return true;
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

#endif

bool UDialogueGraph::ContainsNode(const UDialogueGraphNode* GraphNode) const
{
    return Nodes.Contains(GraphNode->GetNodeID());
}

const UDialogueGraphNode* UDialogueGraph::GetNode(const FGuid NodeID) const
{
    if (!Nodes.Contains(NodeID))
    {
        return nullptr;
    }
    return Nodes[NodeID];
}
