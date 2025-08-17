// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class IRuntimeDialogueGraphVisitor;
class UDialogueCustomNode;

class DIALOGUESYSTEMRUNTIME_API FDialogueSystemCustomNodeDefinition
{
public:
    virtual ~FDialogueSystemCustomNodeDefinition() = default;

    // Getter/Setter
    virtual TSubclassOf<UDialogueCustomNode> GetRuntimeCustomNodeType() const = 0;

    virtual void EvaluateCustomNode(const UDialogueCustomNode* CustomNode, UObject* Evaluator) const = 0;

protected:
    TSubclassOf<UDialogueCustomNode> RuntimeCustomNodeType;
};
