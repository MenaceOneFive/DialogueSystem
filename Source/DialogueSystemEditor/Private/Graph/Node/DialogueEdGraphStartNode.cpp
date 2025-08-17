#include "Graph/Node/DialogueEdGraphStartNode.h"
#include "EdGraph/EdGraph.h"
#include "Graph/Node/DialogueStartNode.h"
#include "Visitor/AbstractDialogueEdGraphVisitor.h"

UDialogueEdGraphStartNode::UDialogueEdGraphStartNode()
{
    DialogueSetting.bShouldShowCursor      = false;
    DialogueSetting.EFocusMode             = EFocusMode::UI;
    DialogueSetting.bCanVisitMultipleTimes = false;
}

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

void UDialogueEdGraphStartNode::CopyTo(const TObjectPtr<UDialogueStartNode>& StartNode) const
{
    StartNode->SetWhenSelectThisNodeFunctionName(GetWhenSelectThisNodeFunctionName());
    StartNode->SetDialogueSetting(DialogueSetting);
}

TObjectPtr<const UDialogueEdGraphNode> UDialogueEdGraphStartNode::GetNextNode() const
{
    return ::GetNextNode<ThisClass>(this);
}
