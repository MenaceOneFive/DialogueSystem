// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/Node/DialogueEdGraphKnotNode.h"

bool UDialogueEdGraphKnotNode::ShouldDrawNodeAsControlPointOnly(int32& OutInputPinIndex, int32& OutOutputPinIndex) const
{
    OutInputPinIndex  = 0;
    OutOutputPinIndex = 1;
    for (int32 i = 0; i < Pins.Num(); ++i)
    {
        if (Pins[i]->Direction == EGPD_Input)
        {
            OutInputPinIndex = i;
        }
        if (Pins[i]->Direction == EGPD_Output)
        {
            OutOutputPinIndex = i;
        }
    }
    return true;
}

void UDialogueEdGraphKnotNode::AllocateDefaultPins()
{
    // Super::AllocateDefaultPins();
    const FName InputPinName(TEXT("InputPin"));
    const FName OutputPinName(TEXT("OutputPin"));

    UEdGraphPin* MyInputPin            = CreatePin(EGPD_Input, PC_Wildcard, InputPinName);
    MyInputPin->bDefaultValueIsIgnored = true;

    CreatePin(EGPD_Output, PC_Wildcard, OutputPinName);
}

TArray<TObjectPtr<const UDialogueEdGraphNode>> UDialogueEdGraphKnotNode::GetPrevNodes() const
{
    const auto InputPin = Algo::FindByPredicate(Pins, [](const UEdGraphPin* Pin)
    {
        return Pin->Direction == EGPD_Input;
    });

    checkf(InputPin, TEXT("InputPin은 무조건 존재해야 합니다."))

    // 한 핀에 들어 올 수 있는 입력은 여러개일 수 있다.
    TArray<TObjectPtr<const UDialogueEdGraphNode>> Output;

    // 입력핀과 연결되어 있는 핀들의 소유주를 목록화 해서 반환
    Algo::Transform((*InputPin)->LinkedTo, Output, [](const UEdGraphPin* Pin)
    {
        return Cast<UDialogueEdGraphNode>(Pin->GetOwningNode());
    });
    return Output;
}

TObjectPtr<const UDialogueEdGraphNode> UDialogueEdGraphKnotNode::GetNextNode() const
{
    // StartNode는 출력 1개만 갖고 있다고 가정한다.

    UEdGraphPin* OutputPin = *Algo::FindByPredicate(Pins, [](const UEdGraphPin* Pin)
    {
        return Pin->Direction == EGPD_Output;
    });

    // 연결이 없는 경우 nullptr를 반환
    if (!OutputPin)
    {
        return nullptr;
    }

    // 출력에 연결되는 입력은 1개만 있다고 가정한다.
    if (!OutputPin->LinkedTo.IsEmpty())
    {
        if (const UEdGraphPin* LinkedTo = *(OutputPin->LinkedTo.begin()))
        {
            return Cast<UDialogueEdGraphNode>(LinkedTo->GetOwningNode());
        }
    }
    return nullptr;
}

UEdGraphPin* UDialogueEdGraphKnotNode::GetInputPin()
{
    const auto InputPin = Algo::FindByPredicate(Pins, [](const UEdGraphPin* Pin)
    {
        return Pin->Direction == EGPD_Input;
    });
    return *InputPin;
}

UEdGraphPin* UDialogueEdGraphKnotNode::GetOutputPin()
{
    const auto OutputPin = Algo::FindByPredicate(Pins, [](const UEdGraphPin* Pin)
    {
        return Pin->Direction == EGPD_Output;
    });
    return *OutputPin;
}
