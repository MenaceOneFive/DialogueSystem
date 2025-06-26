#include "Graph/Node/DialogueEdGraphSceneNode.h"

void UDialogueEdGraphSceneNode::AllocateDefaultPins()
{
    Super::AllocateDefaultPins();
    UEdGraphPin* PrevNodePin = CreatePin(EGPD_Input, PC_Exec, "PrevNode");
    UEdGraphPin* NextNodePin = CreatePin(EGPD_Output, PC_Exec, "NextNode");
    PrevNodePin->bHidden     = false;
    NextNodePin->bHidden     = false;
}
