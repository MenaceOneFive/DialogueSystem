#include "Graph/Node/DialogueEndNode.h"
#include "Graph/DialogueGraphVisitor.h"

void UDialogueEndNode::Accept(TObjectPtr<IRuntimeDialogueGraphVisitor> Visitor) const
{
    Visitor->VisitEndNode(this);
}

TArray<FGuid> UDialogueEndNode::GetPrevNodeIDs() const
{
    return PrevNodeIDs;
}

void UDialogueEndNode::SetPrevNodeIDs(const TArray<FGuid>& InPrevNodeIDs)
{
    this->PrevNodeIDs = InPrevNodeIDs;
}
