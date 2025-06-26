#include "Graph/Node/DialogueEdGraphConditionalNode.h"

FName UDialogueEdGraphConditionalNode::GetCanSelectThisNodeFunctionName() const
{
    return CanSelectThisNodeFunctionName;
}

void UDialogueEdGraphConditionalNode::SetCanSelectThisNodeFunctionName(FName InCanSelectThisNodeFunctionName)
{
    CanSelectThisNodeFunctionName = InCanSelectThisNodeFunctionName;
}
