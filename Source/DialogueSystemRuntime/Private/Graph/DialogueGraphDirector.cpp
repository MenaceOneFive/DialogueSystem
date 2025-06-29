// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/DialogueGraphDirector.h"

#include "Graph/Node/DialogueGraphNode.h"

FName UDialogueGraphDirector::CanSelectThisNodeSignatureName  = "CanSelectThisNodeSignature";
FName UDialogueGraphDirector::WhenSelectThisNodeSignatureName = "WhenSelectThisNodeSignature";

bool UDialogueGraphDirector::CanVisitNode(const TObjectPtr<UFunction>& Function,
                                          const TObjectPtr<const UDialogueGraphNode>& NextNodeToVisit)
{
    checkf(IsValid(Function), TEXT("Function은 nullptr일 수 없습니다."))
    checkf(IsValid(NextNodeToVisit), TEXT("방문할 노드는 nullptr일 수 없습니다."))

    UE_LOG(LogTemp, Log, TEXT("CanVisitNode(NextNodeToVisit: %s)"), *NextNodeToVisit->GetName())

    bool Result;
    InvokeTwoInputBlueprintFunction<bool, const UDialogueGraphNode>(Function, &Result, NextNodeToVisit);
    return Result;
}

void UDialogueGraphDirector::WhenVisitThisNode(const TObjectPtr<UFunction>& Function,
                                               const TObjectPtr<const UDialogueGraphNode>& PrevNode,
                                               const TObjectPtr<const UDialogueGraphNode>& CurrentNode)
{
    checkf(IsValid(Function), TEXT("Function은 nullptr일 수 없습니다."))

    // 시작 노드도 이 메서드를 사용하므로 PrevNode는 nullptr이 될 수 있다.
    checkf(IsValid(CurrentNode), TEXT("CurrentNode는 nullptr일 수 없습니다."))

    if ( !PrevNode )
    {
        UE_LOG(LogTemp, Log, TEXT("WhenVisitThisNode(PrevNode : nullptr, CurrentNode : %s)"), *CurrentNode->GetName())
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("WhenVisitThisNode(PrevNode : %s, CurrentNode : %s)"), *PrevNode->GetName(), *CurrentNode->GetName())
    }

    InvokeTwoInputBlueprintFunction(Function, PrevNode.Get(), CurrentNode.Get());
}
