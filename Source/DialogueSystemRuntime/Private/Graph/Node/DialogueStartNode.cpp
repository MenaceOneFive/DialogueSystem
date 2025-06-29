#include "Graph/Node/DialogueStartNode.h"
#include "Graph/DialogueGraphVisitor.h"

void UDialogueStartNode::Accept(TObjectPtr<IRuntimeDialogueGraphVisitor> Visitor) const
{
    Visitor->VisitStartNode(this);
}

FGuid UDialogueStartNode::GetNextNodeID() const
{
    return NextNodeID;
}

void UDialogueStartNode::SetNextNodeID(const FGuid& InNextNodeID)
{
    this->NextNodeID = InNextNodeID;
}
