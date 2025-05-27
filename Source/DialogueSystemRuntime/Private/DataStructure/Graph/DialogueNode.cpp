#include "DataStructure/Graph/DialogueNode.h"
#include "DataStructure/Graph/DialogueGraphVisitor.h"

FGuid UDialogueGraphNode::GetNodeID() const
{
    return NodeID;
}

FGuid UDialogueGraphNode::GetNextNodeID() const
{
    return NextNodeID;
}

FGuid UDialogueGraphNode::GetPrevNodeID() const
{
    return PrevNodeID;
}

#if WITH_EDITORONLY_DATA

void UDialogueGraphNode::SetNodeID(const FGuid& InNodeGuid)
{
    this->NodeID = InNodeGuid;
}

void UDialogueGraphNode::SetNextNodeID(const FGuid& InNextNodeID)
{
    this->NextNodeID = InNextNodeID;
}

void UDialogueGraphNode::SetPrevNodeID(const FGuid& InPrevNodeID)
{
    this->PrevNodeID = InPrevNodeID;
}

#endif


void UDialogueSelectionNode::VisitNode(IDialogueGraphVisitor* Visitor)
{
    Visitor->VisitSelectionNode(this);
}

TArray<FDialogueSelectionItem> UDialogueSelectionNode::GetSelectionItems() const
{
    return SelectionItems;
}

void UDialogueStartNode::VisitNode(IDialogueGraphVisitor* Visitor)
{
    Visitor->VisitStartNode(this);
}

void UDialogueSelectionNode::SetSelectionItems(const TArray<FDialogueSelectionItem>& Items)
{
    this->SelectionItems = Items;
}

void UDialogueSceneNode::VisitNode(IDialogueGraphVisitor* Visitor)
{
    Visitor->VisitSceneNode(this);
}

ULevelSequence* UDialogueSceneNode::GetLevelSequenceToPlay() const
{
    return LevelSequenceToPlay;
}

void UDialogueSceneNode::SetLevelSequenceToPlay(ULevelSequence* const InLevelSequenceToPlay)
{
    this->LevelSequenceToPlay = InLevelSequenceToPlay;
}

void UDialogueEndNode::VisitNode(IDialogueGraphVisitor* Visitor)
{
    Visitor->VisitEndNode(this);
}

void UDialogueEventNode::VisitNode(IDialogueGraphVisitor* Visitor)
{
    Visitor->VisitEventNode(this);
}
