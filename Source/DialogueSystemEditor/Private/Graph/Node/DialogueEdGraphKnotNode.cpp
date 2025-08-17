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


bool UDialogueEdGraphKnotNode::CanSplitPin(const UEdGraphPin* Pin) const
{
    return false;
}

TArray<TObjectPtr<const UDialogueEdGraphNode>> UDialogueEdGraphKnotNode::GetPrevNodes() const
{
    return ::GetPrevNodes<ThisClass>(this);
}

TObjectPtr<const UDialogueEdGraphNode> UDialogueEdGraphKnotNode::GetNextNode() const
{
    return ::GetNextNode<ThisClass>(this);
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
