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
    virtual FText GetSelectionName() const override;
    virtual FText GetDefaultSelectionName() const override;

#if WITH_EDITORONLY_DATA
    virtual void SetCanSelectThisNodeFunctionName(FName InCanSelectThisNodeFunctionName = NAME_None) override;
    virtual void SetSelectionName(FText& InSelectionName) override;
#endif

protected: // ICanSelectThisNode
    UPROPERTY()
    FName CanSelectThisNodeFunctionName = NAME_None;

    UPROPERTY(EditAnywhere)
    FText SelectionName;
};
