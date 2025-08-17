// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueEdGraphSceneNode.h"
#include "DialogueEdGraphCustomNode.generated.h"

class UDialogueCustomNode;
class UDialogueSystemEvent;


/// <summary>
/// </summary>
UCLASS()
class DIALOGUESYSTEMEDITOR_API UDialogueEdGraphCustomNode : public UDialogueEdGraphSceneNode
{
    GENERATED_BODY()

public:
    virtual void Accept(FAbstractDialogueEdGraphVisitor* Visitor) override;

    virtual void CopyTo(const TObjectPtr<UDialogueCustomNode>& CustomNode) const;
    virtual FText GetDefaultSelectionName() const override;

protected:
};
