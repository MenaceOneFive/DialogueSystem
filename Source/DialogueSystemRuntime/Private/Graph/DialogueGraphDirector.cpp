// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/DialogueGraphDirector.h"

#include "Graph/Node/DialogueGraphNode.h"

FName UDialogueGraphDirector::CanSelectThisNodeSignatureName  = "CanSelectThisNodeSignature";
FName UDialogueGraphDirector::WhenSelectThisNodeSignatureName = "WhenSelectThisNodeSignature";

bool UDialogueGraphDirector::CanVisitNode(const TObjectPtr<UFunction>& Function,
                                          const UDialogueGraphNode* NextNodeToVisit)
{
    check(Function && Function->IsValidLowLevel())
    check(NextNodeToVisit)


    UE_LOG(LogTemp, Log, TEXT("CanVisitNode(NextNodeToVisit: %s)"), *NextNodeToVisit->GetName())

    bool Result;
    InvokeTwoInputBlueprintFunction<bool, const UDialogueGraphNode>(Function, &Result, NextNodeToVisit);
    return Result;
}

void UDialogueGraphDirector::WhenVisitThisNode(const TObjectPtr<UFunction>& Function,
                                               const UDialogueGraphNode* PrevNode,
                                               const UDialogueGraphNode* CurrentNode)
{
    if ( !PrevNode )
    {
        UE_LOG(LogTemp, Log, TEXT("WhenVisitThisNode(PrevNode : nullptr, CurrentNode : %s)"), *CurrentNode->GetName())
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("WhenVisitThisNode(PrevNode : %s, CurrentNode : %s)"), *PrevNode->GetName(), *CurrentNode->GetName())
    }
    InvokeTwoInputBlueprintFunction(Function, PrevNode, CurrentNode);
}
