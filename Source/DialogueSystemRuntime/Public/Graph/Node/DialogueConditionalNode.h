#pragma once
#include "Graph/Node/DialogueGraphNode.h"
#include "Graph/Node/DialogueNodeInterface.h"
#include "DialogueConditionalNode.generated.h"

UCLASS(Abstract, BlueprintType)
class DIALOGUESYSTEMRUNTIME_API UDialogueConditionalNode : public UDialogueGraphNode,
                                                           public ICanSelectThisNode
{
    GENERATED_BODY()

public:
    virtual FName GetCanSelectThisNodeFunctionName() const override;
    virtual FText GetDefaultSelectionName() const override;;
    virtual FText GetSelectionName() const override;

#if WITH_EDITORONLY_DATA
    virtual void SetCanSelectThisNodeFunctionName(FName InCanSelectThisNodeFunctionName = NAME_None) override;
    virtual void SetSelectionName(FText& InSelectionName) override;
#endif

protected:
    UPROPERTY(VisibleAnywhere)
    FName CanSelectThisNodeName = NAME_None;

    UPROPERTY(VisibleAnywhere)
    FText SelectionName;
};
