#include "Graph/Node/DialogueSceneNode.h"
#include "MovieSceneSequence.h"
#include "Graph/DialogueGraphVisitor.h"

void UDialogueSceneNode::Accept(TObjectPtr<IRuntimeDialogueGraphVisitor> Visitor) const
{
    Visitor->VisitSceneNode(this);
}

ULevelSequence* UDialogueSceneNode::GetLevelSequenceToPlay() const
{
    return LevelSequenceToPlay.Get();
}

void UDialogueSceneNode::SetLevelSequenceToPlay(ULevelSequence* const InLevelSequenceToPlay)
{
    this->LevelSequenceToPlay = InLevelSequenceToPlay;
}

bool UDialogueSceneNode::CanSkipThisNode() const
{
    return true;
}

TArray<FGuid> UDialogueSceneNode::GetPrevNodeIDs() const
{
    return PrevNodeIDs;
}

void UDialogueSceneNode::SetPrevNodeIDs(const TArray<FGuid>& InPrevNodeIDs)
{
    this->PrevNodeIDs = InPrevNodeIDs;
}

FGuid UDialogueSceneNode::GetNextNodeID() const
{
    return NextNodeID;
}

void UDialogueSceneNode::SetNextNodeID(const FGuid& InNextNodeID)
{
    this->NextNodeID = InNextNodeID;
}
