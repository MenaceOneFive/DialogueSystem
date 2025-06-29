#include "Graph/Node/DialogueEdGraphEndNode.h"
#include "EdGraph/EdGraph.h"
#include "Graph/DialogueEdGraphVisitor.h"

FText UDialogueEdGraphEndNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    return FText::FromString(TEXT("대화 끝"));
}

void UDialogueEdGraphEndNode::AllocateDefaultPins()
{
    Super::AllocateDefaultPins();
    UEdGraphPin* PrevNodePin = CreatePin(EGPD_Input, PC_Exec, "PrevNode");
    PrevNodePin->bHidden     = false;
}

void UDialogueEdGraphEndNode::Accept(FAbstractDialogueEdGraphVisitor* Visitor)
{
    Visitor->VisitEndNode(this);
}

TArray<TObjectPtr<const UDialogueEdGraphNode>> UDialogueEdGraphEndNode::GetPrevNodes() const
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
