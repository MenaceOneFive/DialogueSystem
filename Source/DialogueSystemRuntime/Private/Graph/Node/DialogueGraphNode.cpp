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

void UDialogueGraphNode::SetNodeID(const FGuid& InNodeGuid)
{
    this->NodeID = InNodeGuid;
}

FName UDialogueGraphNode::GetWhenSelectThisNodeFunctionName() const
{
    return WhenSelectThisNodeFunctionName;
}

void UDialogueGraphNode::SetWhenSelectThisNodeFunctionName(FName InWhenSelectThisNodeFunctionName)
{
    WhenSelectThisNodeFunctionName = InWhenSelectThisNodeFunctionName;
}
