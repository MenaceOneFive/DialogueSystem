#pragma once

#include "CoreMinimal.h"
#include "Graph/Node/DialogueEdGraphNode.h"
#include "Graph/Node/DialogueGraphNode.h" // For ICanSelectThisNode
#include "DialogueEdGraphConditionalNode.generated.h"

UCLASS(Abstract)
class DIALOGUESYSTEMEDITOR_API UDialogueEdGraphConditionalNode : public UDialogueEdGraphNode,
                                                                 public ICanSelectThisNode
{
    GENERATED_BODY()

public: // ICanSelectThisNode
    virtual FName GetCanSelectThisNodeFunctionName() const override;
    virtual void SetCanSelectThisNodeFunctionName(FName InCanSelectThisNodeFunctionName = NAME_None) override;

protected: // ICanSelectThisNode
    UPROPERTY()
    FName CanSelectThisNodeFunctionName = NAME_None;
};
