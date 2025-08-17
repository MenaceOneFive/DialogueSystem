#include "Graph/Node/DialogueEdGraphEndNode.h"
#include "EdGraph/EdGraph.h"
#include "Graph/Node/DialogueEndNode.h"
#include "Visitor/AbstractDialogueEdGraphVisitor.h"

UDialogueEdGraphEndNode::UDialogueEdGraphEndNode()
{
    DialogueSetting.bShouldShowCursor = false;
    DialogueSetting.EFocusMode        = EFocusMode::Game;
}

FText UDialogueEdGraphEndNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    return FText::FromString(TEXT("대화 끝"));
}

void UDialogueEdGraphEndNode::AllocateDefaultPins()
{
    Super::AllocateDefaultPins();
    UEdGraphPin* PrevNodePin = CreatePin(EGPD_Input, PC_Exec, "PrevNode");
    PrevNodePin->bHidden     = false;
}

void UDialogueEdGraphEndNode::Accept(FAbstractDialogueEdGraphVisitor* Visitor)
{
    Visitor->VisitEndNode(this);
}

void UDialogueEdGraphEndNode::CopyTo(const TObjectPtr<UDialogueEndNode>& EndNode) const
{
    EndNode->SetWhenSelectThisNodeFunctionName(GetWhenSelectThisNodeFunctionName());
    EndNode->SetDialogueSetting(DialogueSetting);
}

TArray<TObjectPtr<const UDialogueEdGraphNode>> UDialogueEdGraphEndNode::GetPrevNodes() const
{
    return ::GetPrevNodes<ThisClass>(this);
}
