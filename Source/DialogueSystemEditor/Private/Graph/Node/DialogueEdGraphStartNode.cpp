#include "Graph/Node/DialogueEdGraphStartNode.h"
#include "EdGraph/EdGraph.h"
#include "Visitor/AbstractDialogueEdGraphVisitor.h"

FText UDialogueEdGraphStartNode::GetNodeTitle(const ENodeTitleType::Type TitleType) const
{
    return FText::FromString(TEXT("대화 시작"));
}

void UDialogueEdGraphStartNode::AllocateDefaultPins()
{
    Super::AllocateDefaultPins();
    UEdGraphPin* NextNodePin = CreatePin(EGPD_Output, PC_Exec, "NextNode");
    NextNodePin->bHidden     = false;
}

void UDialogueEdGraphStartNode::Accept(FAbstractDialogueEdGraphVisitor* Visitor)
{
    Visitor->VisitStartNode(this);
}

TObjectPtr<const UDialogueEdGraphNode> UDialogueEdGraphStartNode::GetNextNode() const
{
    // StartNode는 출력 1개만 갖고 있다고 가정한다.
    UEdGraphPin* PinToNext = (*Pins.begin());

    // 연결이 없는 경우 nullptr를 반환
    if ( !PinToNext )
    {
        return nullptr;
    }

    // 출력에 연결되는 입력은 1개만 있다고 가정한다.
    if ( !PinToNext->LinkedTo.IsEmpty() )
    {
        if ( const UEdGraphPin* LinkedTo = *(PinToNext->LinkedTo.begin()) )
        {
            return Cast<UDialogueEdGraphNode>(LinkedTo->GetOwningNode());
        }
    }
    return nullptr;
}
