#include "Graph/Node/DialogueSelectionItem.h"

bool UDialogueSelectionItem::CanSelectThisItem() const
{
    return bCanSelectThisItem;
}

void UDialogueSelectionItem::SelectThisItem() const
{
    check(VisitNextNodeDelegate.IsBound())
    (void)VisitNextNodeDelegate.ExecuteIfBound();
}

void UDialogueSelectionItem::Initialize(const FSimpleDelegate& WhenSelectThisNodeDelegate, bool CanSelectThisNode)
{
    VisitNextNodeDelegate = WhenSelectThisNodeDelegate;
    bCanSelectThisItem    = CanSelectThisNode;
}

void UDialogueSelectionItem::SetSelectionName(const FText& InSelectionName)
{
    this->SelectionName = InSelectionName;
}
