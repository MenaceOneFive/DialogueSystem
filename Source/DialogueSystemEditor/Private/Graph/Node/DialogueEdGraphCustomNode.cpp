// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/Node/DialogueEdGraphCustomNode.h"

#include "Graph/Node/DialogueCustomNode.h"
#include "Visitor/AbstractDialogueEdGraphVisitor.h"

void UDialogueEdGraphCustomNode::Accept(FAbstractDialogueEdGraphVisitor* Visitor)
{
    Visitor->VisitCustomNode(this);
}

void UDialogueEdGraphCustomNode::CopyTo(const TObjectPtr<UDialogueCustomNode>& CustomNode) const
{
    CustomNode->SetWhenSelectThisNodeFunctionName(GetWhenSelectThisNodeFunctionName());
    CustomNode->SetCanSelectThisNodeFunctionName(GetCanSelectThisNodeFunctionName());
    auto Name = GetSelectionName();
    CustomNode->SetSelectionName(Name);

    CustomNode->SetDialogueSetting(GetDialogueSetting());
}

FText UDialogueEdGraphCustomNode::GetDefaultSelectionName() const
{
    return NSLOCTEXT("DialoguePlayer", "CustomNode", "커스텀 노드");
}
