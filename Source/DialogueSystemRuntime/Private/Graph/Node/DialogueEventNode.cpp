#include "Graph/Node/DialogueEventNode.h"
#include "Graph/DialogueGraphVisitor.h"

void UDialogueEventNode::Accept(TObjectPtr<IRuntimeDialogueGraphVisitor> Visitor) const
{
    Visitor->VisitEventNode(this);
}

TArray<FGuid> UDialogueEventNode::GetPrevNodeIDs() const
{
    return PrevNodeIDs;
}

void UDialogueEventNode::SetPrevNodeIDs(const TArray<FGuid>& InPrevNodeIDs)
{
    this->PrevNodeIDs = InPrevNodeIDs;
}

FGuid UDialogueEventNode::GetNextNodeID() const
{
    return NextNodeID;
}

void UDialogueEventNode::SetNextNodeID(const FGuid& InNextNodeID)
{
    this->NextNodeID = InNextNodeID;
}
