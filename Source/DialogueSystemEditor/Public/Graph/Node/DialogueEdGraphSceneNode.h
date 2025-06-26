#pragma once

#include "CoreMinimal.h"
#include "Graph/Node/DialogueEdGraphConditionalNode.h"
#include "DialogueEdGraphSceneNode.generated.h"

UCLASS()
class DIALOGUESYSTEMEDITOR_API UDialogueEdGraphSceneNode : public UDialogueEdGraphConditionalNode
{
    GENERATED_BODY()

public: // UEdGraphNode
    virtual void AllocateDefaultPins() override;
};
