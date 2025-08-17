#include "Graph/Node/DialogueEdGraphConditionalNode.h"

FName UDialogueEdGraphConditionalNode::GetCanSelectThisNodeFunctionName() const
{
    return CanSelectThisNodeFunctionName;
}

FText UDialogueEdGraphConditionalNode::GetSelectionName() const
{
    auto DefaultSelectionName = GetDefaultSelectionName();
    return !SelectionName.IsEmpty() ? SelectionName : DefaultSelectionName;
}

FText UDialogueEdGraphConditionalNode::GetDefaultSelectionName() const
{
    return NSLOCTEXT("DialoguePlayer", "SelectionItemNode", "다른 선택");
}

#if WITH_EDITORONLY_DATA
void UDialogueEdGraphConditionalNode::SetSelectionName(FText& InSelectionName)
{
    SelectionName = InSelectionName;
}

void UDialogueEdGraphConditionalNode::SetCanSelectThisNodeFunctionName(FName InCanSelectThisNodeFunctionName)
{
    CanSelectThisNodeFunctionName = InCanSelectThisNodeFunctionName;
}
#endif
