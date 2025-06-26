#include "Graph/Node/DialogueConditionalNode.h"

FName UDialogueConditionalNode::GetCanSelectThisNodeFunctionName() const
{
    return CanSelectThisNodeName;
}

void UDialogueConditionalNode::SetCanSelectThisNodeFunctionName(FName InCanSelectThisNodeFunctionName)
{
    CanSelectThisNodeName = InCanSelectThisNodeFunctionName;
}
