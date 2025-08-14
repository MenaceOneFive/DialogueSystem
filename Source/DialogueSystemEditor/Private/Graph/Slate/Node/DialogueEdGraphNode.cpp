// Fill out your copyright notice in the Description page of Project Settings.
#include "Graph/Node/DialogueEdGraphNode.h"
#include "EdGraph/EdGraph.h"

#define LOCTEXT_NAMESPACE "FDialogueGraphEditor"

const FName UDialogueEdGraphNode::PC_Flow(TEXT("Flow"));
const FName UDialogueEdGraphNode::PC_Exec(TEXT("Exec"));
const FName UDialogueEdGraphNode::PC_Wildcard(TEXT("wildcard"));

UDialogueEdGraphNode::UDialogueEdGraphNode()
{
    bCanRenameNode = true;
    bCanResizeNode = true;
}

bool UDialogueEdGraphNode::IsAllowRecursiveConnection() const
{
    return bAllowRecursiveConnection;
}

void UDialogueEdGraphNode::PostInitProperties()
{
    Super::PostInitProperties();
    // GUID가 없으면 정상적으로 동작하지 않는다.
    CreateNewGuid();
}

void UDialogueEdGraphNode::PostPlacedNewNode()
{
    Super::PostPlacedNewNode();
}

void UDialogueEdGraphNode::AutowireNewNode(UEdGraphPin* FromPin)
{
    if (!FromPin)
    {
        return;
    }

    // 아다리가 맞는 같은 타입의 핀을 찾아서 연결을 수행한다.
    // 코드가 중복되는 부분이 있지만 분리하면 더 복잡해서 이렇게 작성했다.
    TArray<UEdGraphPin*> EdGraphPins = GetAllPins();
    if (FromPin->Direction == EGPD_Input)
    {
        UEdGraphPin** PinToConnect = EdGraphPins.FindByPredicate([FromPin](const UEdGraphPin* Pin)
        {
            // 같은 핀의 유형이면서 다른 출력방향
            return Pin->Direction == EGPD_Output &&
                   Pin->PinType == FromPin->PinType;
        });
        if (PinToConnect)
        {
            const bool bConnectionResult = GetSchema()->TryCreateConnection(FromPin, *PinToConnect);
            checkf(bConnectionResult, TEXT("연결이 성립될 수 없습니다."));
        }
    }
    else if (FromPin->Direction == EGPD_Output)
    {
        UEdGraphPin** PinToConnect = EdGraphPins.FindByPredicate([FromPin](const UEdGraphPin* Pin)
        {
            // 같은 핀의 유형이면서 다른 출력방향
            return Pin->Direction == EGPD_Input &&
                   Pin->PinType == FromPin->PinType;
        });
        if (PinToConnect)
        {
            const bool bConnectionResult = GetSchema()->TryCreateConnection(FromPin, *PinToConnect);
            checkf(bConnectionResult, TEXT("연결이 성립될 수 없습니다."));
        }
    }
}

void UDialogueEdGraphNode::SetNodePosition(const int32 InPosX,
                                           const int32 InPosY)
{
    NodePosX = InPosX;
    NodePosY = InPosY;
}

FName UDialogueEdGraphNode::GetWhenSelectThisNodeFunctionName() const
{
    return WhenSelectThisNodeFunctionName;
}

void UDialogueEdGraphNode::SetWhenSelectThisNodeFunctionName(FName InWhenSelectThisNodeFunctionName)
{
    WhenSelectThisNodeFunctionName = InWhenSelectThisNodeFunctionName;
}

#undef LOCTEXT_NAMESPACE
