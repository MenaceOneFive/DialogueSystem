#include "Graph/Node/DialogueLineNode.h"
#include "MovieSceneSequence.h"
#include "Graph/DialogueGraphVisitor.h"

void UDialogueLineNode::Accept(TScriptInterface<IRuntimeDialogueGraphVisitor> Visitor) const
{
    Visitor->VisitSceneNode(this);
}

FText UDialogueLineNode::GetDefaultSelectionName() const
{
    return NSLOCTEXT("DialoguePlayer", "LineNode", "대사 노드");
}

TSoftObjectPtr<ULevelSequence> UDialogueLineNode::GetLevelSequenceToPlay() const
{
    return LevelSequenceToPlay;
}

FSequencePlaySetting UDialogueLineNode::GetSequencePlaySetting() const
{
    return SequencePlaySetting;
}

TArray<FGuid> UDialogueLineNode::GetPrevNodeIDs() const
{
    return PrevNodeIDs;
}

FGuid UDialogueLineNode::GetNextNodeID() const
{
    return NextNodeID;
}

#if WITH_EDITORONLY_DATA
void UDialogueLineNode::SetLevelSequenceToPlay(const TSoftObjectPtr<ULevelSequence>& InLevelSequenceToPlay)
{
    this->LevelSequenceToPlay = InLevelSequenceToPlay;
}

void UDialogueLineNode::SetSequencePlaySetting(const FSequencePlaySetting& InSequencePlaySetting)
{
    this->SequencePlaySetting = InSequencePlaySetting;
}


void UDialogueLineNode::SetPrevNodeIDs(const TArray<FGuid>& InPrevNodeIDs)
{
    this->PrevNodeIDs = InPrevNodeIDs;
}

void UDialogueLineNode::SetNextNodeID(const FGuid& InNextNodeID)
{
    this->NextNodeID = InNextNodeID;
}
#endif

bool UDialogueLineNode::CanSkipThisNode() const
{
    return SequencePlaySetting.bCanSkipThisNode;
}
