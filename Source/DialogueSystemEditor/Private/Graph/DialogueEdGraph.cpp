// Fill out your copyright notice in the Description page of Project Settings.
#include "Graph/DialogueEdGraph.h"

TArray<TObjectPtr<UDialogueEdGraphNode>> UDialogueEdGraph::GetNodes() const
{
    TArray<TObjectPtr<UDialogueEdGraphNode>> Result;
    Algo::Transform(Nodes, Result, [](const TObjectPtr<UEdGraphNode>& Node)
    {
        return Cast<UDialogueEdGraphNode>(Node.Get());
    });
    return Result;
}
