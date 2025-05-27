// Fill out your copyright notice in the Description page of Project Settings.
#include "Graph/DialogueEdGraphNodes.h"

#define LOCTEXT_NAMESPACE "FDialogueGraphEditor"

const FName UDialogueEdGraphNode::PC_Flow(TEXT("Flow"));
const FName UDialogueEdGraphNode::PC_Exec(TEXT("Exec"));

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
    // 기본 핀 생성
    AllocateDefaultPins();
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
            bool bConnectionResult = GetSchema()->TryCreateConnection(FromPin, *PinToConnect);
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
            bool bConnectionResult = GetSchema()->TryCreateConnection(FromPin, *PinToConnect);
            checkf(bConnectionResult, TEXT("연결이 성립될 수 없습니다."));
        }
    }
}

void UDialogueEdGraphNode::SetNodePosition(int32 InPosX,
                                           int32 InPosY)
{
    NodePosX = InPosX;
    NodePosY = InPosY;
}


void UDialogueEdGraphStartNode::AllocateDefaultPins()
{
    Super::AllocateDefaultPins();
    UEdGraphPin* NextNodePin = CreatePin(EGPD_Output, PC_Exec, "NextNode");
    NextNodePin->bHidden     = false;
}

void UDialogueEdGraphEndNode::AllocateDefaultPins()
{
    Super::AllocateDefaultPins();
    UEdGraphPin* PrevNodePin = CreatePin(EGPD_Input, PC_Exec, "PrevNode");
    PrevNodePin->bHidden     = false;
}

void UDialogueEdGraphSceneNode::AllocateDefaultPins()
{
    Super::AllocateDefaultPins();
    UEdGraphPin* PrevNodePin = CreatePin(EGPD_Input, PC_Exec, "PrevNode");
    UEdGraphPin* NextNodePin = CreatePin(EGPD_Output, PC_Exec, "NextNode");
    PrevNodePin->bHidden     = false;
    NextNodePin->bHidden     = false;
}

void UDialogueEdGraphSelectNode::AllocateDefaultPins()
{
    Super::AllocateDefaultPins();
    UEdGraphPin* PrevNodePin  = CreatePin(EGPD_Input, PC_Exec, "PrevNode");
    PrevNodePin->bHidden      = false;
    UEdGraphPin* NextNodePin1 = CreatePin(EGPD_Output, PC_Exec, FName(FString::Format(TEXT("NextNode{0}"), {++PinCounter})));
    NextNodePin1->bHidden     = false;
    UEdGraphPin* NextNodePin2 = CreatePin(EGPD_Output, PC_Exec, FName(FString::Format(TEXT("NextNode{0}"), {++PinCounter})));
    NextNodePin2->bHidden     = false;
    UEdGraphPin* NextNodePin3 = CreatePin(EGPD_Output, PC_Exec, FName(FString::Format(TEXT("NextNode{0}"), {++PinCounter})));
    NextNodePin3->bHidden     = false;
}

void UDialogueEdGraphSelectNode::AddOutputPin()
{
    for (const UEdGraphPin* Pin : Pins)
    {
        // 사용중이지 않은 핀이 있는 경우 새로 핀을 추가하지 않음
        if (!Pin->IsPendingKill() && Pin->LinkedTo.IsEmpty())
        {
            return;
        }
    }
    const FScopedTransaction Transaction(LOCTEXT("AddPinToNode", "Adding Pin to SelectionNode"));
    UEdGraphPin* NextNodePin = CreatePin(EGPD_Output, PC_Exec, FName(FString::Format(TEXT("NextNode{0}"), {++PinCounter})));
    NextNodePin->bHidden     = false;
}

#undef LOCTEXT_NAMESPACE
