#include "Graph/Node/DialogueCustomNode.h"
#include "Graph/DialogueGraphVisitor.h"

void UDialogueCustomNode::Accept(TScriptInterface<IRuntimeDialogueGraphVisitor> Visitor) const
{
    Visitor->VisitCustomNode(this);
}

TSubclassOf<UDialogueSystemEvent> UDialogueCustomNode::GetEventClass() const
{
    return EventClass;
}


#if WITH_EDITORONLY_DATA
void UDialogueCustomNode::SetEventClass(const TSubclassOf<UDialogueSystemEvent>& InEventClass)
{
    this->EventClass = InEventClass;
}
#endif

TArray<FGuid> UDialogueCustomNode::GetPrevNodeIDs() const
{
    return PrevNodeIDs;
}

FGuid UDialogueCustomNode::GetNextNodeID() const
{
    return NextNodeID;
}

FText UDialogueCustomNode::GetDefaultSelectionName() const
{
    return NSLOCTEXT("DialoguePlayer", "CustomNode", "커스텀 노드");
}

#if WITH_EDITORONLY_DATA
void UDialogueCustomNode::SetPrevNodeIDs(const TArray<FGuid>& InPrevNodeIDs)
{
    this->PrevNodeIDs = InPrevNodeIDs;
}

void UDialogueCustomNode::SetNextNodeID(const FGuid& InNextNodeID)
{
    this->NextNodeID = InNextNodeID;
}
#endif

FSequencePlaySetting UDialogueCustomNode::GetSequencePlaySetting() const
{
    return SequencePlaySetting;
}
