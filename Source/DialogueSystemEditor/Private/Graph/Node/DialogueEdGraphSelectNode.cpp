#include "Graph/Node/DialogueEdGraphSelectNode.h"
#include "EdGraph/EdGraph.h"
#include "Visitor/AbstractDialogueEdGraphVisitor.h"

#define LOCTEXT_NAMESPACE "FDialogueGraphEditor"

FText UDialogueEdGraphSelectNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    return FText::FromString(TEXT("선택지"));
}

void UDialogueEdGraphSelectNode::AllocateDefaultPins()
{
    Super::AllocateDefaultPins();
    UEdGraphPin* PrevNodePin  = CreatePin(EGPD_Input, PC_Exec, "PrevNode");
    PrevNodePin->bHidden      = false;
    UEdGraphPin* NextNodePin1 = CreatePin(EGPD_Output, PC_Exec, FName(FString::Format(TEXT("선택지 {0}"), {++PinCounter})));
    NextNodePin1->bHidden     = false;
    UEdGraphPin* NextNodePin2 = CreatePin(EGPD_Output, PC_Exec, FName(FString::Format(TEXT("선택지 {0}"), {++PinCounter})));
    NextNodePin2->bHidden     = false;
    UEdGraphPin* NextNodePin3 = CreatePin(EGPD_Output, PC_Exec, FName(FString::Format(TEXT("선택지 {0}"), {++PinCounter})));
    NextNodePin3->bHidden     = false;
}

FText UDialogueEdGraphSelectNode::GetTooltipText() const
{
    return FText::FromString(TEXT("플레이어에게 주는 선택지"));
}

void UDialogueEdGraphSelectNode::AddOutputPin()
{
    for ( const UEdGraphPin* Pin : Pins )
    {
        // 사용중이지 않은 핀이 있는 경우 새로 핀을 추가하지 않음
        if ( !Pin->IsPendingKill() && Pin->LinkedTo.IsEmpty() )
        {
            return;
        }
    }
    const FScopedTransaction Transaction(LOCTEXT("AddPinToNode", "Adding Pin to Selection Node"));
    UEdGraphPin* NextNodePin = CreatePin(EGPD_Output, PC_Exec, FName(FString::Format(TEXT("선택지 {0}"), {++PinCounter})));
    NextNodePin->bHidden     = false;
}

void UDialogueEdGraphSelectNode::Accept(FAbstractDialogueEdGraphVisitor* Visitor)
{
    Visitor->VisitSelectNode(this);
}

TArray<TObjectPtr<const UDialogueEdGraphNode>> UDialogueEdGraphSelectNode::GetNextNodes() const
{
    // 출력핀만 필터링한다.
    TArray<UEdGraphPin*> OutputPins;
    Algo::CopyIf(Pins, OutputPins, [](const UEdGraphPin* Pin)
    {
        return Pin->Direction == EGPD_Output;
    });

    // 핀에 해당하는 노드를 찾아서 배열에 추가한다.
    TArray<TObjectPtr<const UDialogueEdGraphNode>> OutputNodes;
    for ( UEdGraphPin* OutputPin : OutputPins )
    {
        if ( OutputPin->LinkedTo.IsEmpty() )
        {
            continue;
        }
        if ( const auto LinkedTo = *OutputPin->LinkedTo.begin();
            LinkedTo                     /** 1 대 1 대응이라 가정, 첫번째 핀만 유효하다고 판정 */
            && LinkedTo->GetOwningNode() /** 그 핀은 연결된 상태여야한다. */
        )
        {
            if ( auto OwningNode = Cast<UDialogueEdGraphNode>(LinkedTo->GetOwningNode()) )
            {
                OutputNodes.Emplace(OwningNode);
            }
        }
    }

    return OutputNodes;
}

TArray<TObjectPtr<const UDialogueEdGraphNode>> UDialogueEdGraphSelectNode::GetPrevNodes() const
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

#undef LOCTEXT_NAMESPACE
