#include "Graph/Node/DialogueConditionalNode.h"

FText UDialogueConditionalNode::GetDefaultSelectionName() const
{
    return NSLOCTEXT("DialoguePlayer", "SelectionItemNode", "선택지 노드");
}

FName UDialogueConditionalNode::GetCanSelectThisNodeFunctionName() const
{
    return CanSelectThisNodeName;
}

FText UDialogueConditionalNode::GetSelectionName() const
{
    auto DefaultSelectionName = GetDefaultSelectionName();
    return !SelectionName.IsEmpty() ? SelectionName : DefaultSelectionName;
}

#if WITH_EDITORONLY_DATA
void UDialogueConditionalNode::SetSelectionName(FText& InSelectionName)
{
    SelectionName = InSelectionName;
}

void UDialogueConditionalNode::SetCanSelectThisNodeFunctionName(FName InCanSelectThisNodeFunctionName)
{
    CanSelectThisNodeName = InCanSelectThisNodeFunctionName;
}
#endif
