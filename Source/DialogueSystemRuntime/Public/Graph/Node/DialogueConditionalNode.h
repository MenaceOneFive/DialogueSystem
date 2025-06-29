#pragma once
#include "Graph/Node/DialogueGraphNode.h"
#include "Graph/Node/DialogueNodeInterface.h"
#include "DialogueConditionalNode.generated.h"

UCLASS(BlueprintType)
class DIALOGUESYSTEMRUNTIME_API UDialogueConditionalNode : public UDialogueGraphNode,
                                                           public ICanSelectThisNode
{
    GENERATED_BODY()

public:
    virtual FName GetCanSelectThisNodeFunctionName() const override;
    virtual void SetCanSelectThisNodeFunctionName(FName InCanSelectThisNodeFunctionName = NAME_None) override;

protected:
    UPROPERTY(VisibleAnywhere)
    FName CanSelectThisNodeName = NAME_None;
};
