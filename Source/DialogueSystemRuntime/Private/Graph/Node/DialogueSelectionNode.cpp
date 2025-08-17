#include "Graph/Node/DialogueSelectionNode.h"
#include "Graph/DialogueGraphVisitor.h"
#include "Graph/Node/DialogueSelectionItem.h"

void UDialogueSelectionNode::Accept(TScriptInterface<IRuntimeDialogueGraphVisitor> Visitor) const
{
    Visitor->VisitSelectionNode(this);
}

FText UDialogueSelectionNode::GetDefaultSelectionName() const
{
    return NSLOCTEXT("DialoguePlayer", "SelectNode", "다른 선택");
}

TArray<UDialogueSelectionItem*> UDialogueSelectionNode::GetSelectionItems() const
{
    return SelectionItems;
}

#if WITH_EDITORONLY_DATA
void UDialogueSelectionNode::SetSelectionItems(const TArray<UDialogueSelectionItem*>& Items)
{
    this->SelectionItems.Empty();
    for (UDialogueSelectionItem* Item : Items)
    {
        this->SelectionItems.Add(Item);
    }
}
#endif

TArray<FGuid> UDialogueSelectionNode::GetPrevNodeIDs() const
{
    return PrevNodeIDs;
}

TArray<FGuid> UDialogueSelectionNode::GetNextNodeIDs() const
{
    return NextNodeIDs;
}

FText UDialogueSelectionNode::GetSelectionName() const
{
    return SelectionName;
}

#if WITH_EDITORONLY_DATA
void UDialogueSelectionNode::SetSelectionName(FText& InSelectionName)
{
    SelectionName = InSelectionName;
}

void UDialogueSelectionNode::SetPrevNodeIDs(const TArray<FGuid>& InPrevNodeIDs)
{
    this->PrevNodeIDs = InPrevNodeIDs;
}

void UDialogueSelectionNode::SetNextNodeIDs(const TArray<FGuid>& InNextNodeIDs)
{
    this->NextNodeIDs = InNextNodeIDs;
}
#endif
