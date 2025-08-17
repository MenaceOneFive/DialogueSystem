#include "Graph/Node/DialogueEdGraphSceneNode.h"

#include "Visitor/AbstractDialogueEdGraphVisitor.h"

FText UDialogueEdGraphSceneNode::GetDefaultSelectionName() const
{
    return NSLOCTEXT("DialoguePlayer", "SceneNode", "장면 노드");
}

void UDialogueEdGraphSceneNode::AllocateDefaultPins()
{
    Super::AllocateDefaultPins();
    UEdGraphPin* PrevNodePin = CreatePin(EGPD_Input, PC_Exec, "PrevNode");
    UEdGraphPin* NextNodePin = CreatePin(EGPD_Output, PC_Exec, "NextNode");
    PrevNodePin->bHidden     = false;
    NextNodePin->bHidden     = false;
}

void UDialogueEdGraphSceneNode::Accept(FAbstractDialogueEdGraphVisitor* Visitor)
{
    Visitor->VisitSceneNode(this);
}

TArray<TObjectPtr<const UDialogueEdGraphNode>> UDialogueEdGraphSceneNode::GetPrevNodes() const
{
    return ::GetPrevNodes<ThisClass>(this);
}

TObjectPtr<const UDialogueEdGraphNode> UDialogueEdGraphSceneNode::GetNextNode() const
{
    return ::GetNextNode<ThisClass>(this);
}
