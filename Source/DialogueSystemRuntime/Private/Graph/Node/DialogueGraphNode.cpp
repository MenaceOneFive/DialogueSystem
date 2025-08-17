#include "Graph/Node/DialogueGraphNode.h"

UDialogueGraphNode::UDialogueGraphNode()
{
    NodeID = FGuid::NewGuid();
}

void UDialogueGraphNode::BeginDestroy()
{
    UE_LOG(LogMemory, Display, TEXT("%s %s"), *this->GetName(), TEXT(__FUNCTION__))
    Super::BeginDestroy();
}

FGuid UDialogueGraphNode::GetNodeID() const
{
    return NodeID;
}


FDialogueSetting UDialogueGraphNode::GetDialogueSetting() const
{
    return DialogueSetting;
}

#if WITH_EDITORONLY_DATA

void UDialogueGraphNode::SetNodeID(const FGuid& InNodeGuid)
{
    this->NodeID = InNodeGuid;
}

void UDialogueGraphNode::SetDialogueSetting(const FDialogueSetting& InDialogueSetting)
{
    this->DialogueSetting = InDialogueSetting;
}

void UDialogueGraphNode::SetWhenSelectThisNodeFunctionName(FName InWhenSelectThisNodeFunctionName)
{
    WhenSelectThisNodeFunctionName = InWhenSelectThisNodeFunctionName;
}

#endif

FName UDialogueGraphNode::GetWhenSelectThisNodeFunctionName() const
{
    return WhenSelectThisNodeFunctionName;
}
