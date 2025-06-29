#include "Graph/Node/DialogueSelectionNode.h"
#include "Graph/DialogueGraphVisitor.h"
#include "Graph/Node/DialogueSelectionItem.h"

void UDialogueSelectionNode::Accept(TObjectPtr<IRuntimeDialogueGraphVisitor> Visitor) const
{
    Visitor->VisitSelectionNode(this);
}

TArray<UDialogueSelectionItem*> UDialogueSelectionNode::GetSelectionItems() const
{
    return SelectionItems;
}

void UDialogueSelectionNode::SetSelectionItems(const TArray<UDialogueSelectionItem*>& Items)
{
    this->SelectionItems.Empty();
    for ( UDialogueSelectionItem* Item : Items )
    {
        this->SelectionItems.Add(Item);
    }
}

TArray<FGuid> UDialogueSelectionNode::GetPrevNodeIDs() const
{
    return PrevNodeIDs;
}

void UDialogueSelectionNode::SetPrevNodeIDs(const TArray<FGuid>& InPrevNodeIDs)
{
    this->PrevNodeIDs = InPrevNodeIDs;
}

TArray<FGuid> UDialogueSelectionNode::GetNextNodeIDs() const
{
    return NextNodeIDs;
}

void UDialogueSelectionNode::SetNextNodeIDs(const TArray<FGuid>& InNextNodeIDs)
{
    this->NextNodeIDs = InNextNodeIDs;
}
