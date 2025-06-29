// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DialoguePositionController.generated.h"

class UDialogueGraphNode;
// This class does not need to be modified.
UINTERFACE()
class UDialoguePositionController : public UInterface
{
    GENERATED_BODY()
};

/**
 * 
 */
class DIALOGUESYSTEMRUNTIME_API IDialoguePositionController
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    virtual TObjectPtr<UDialogueGraphNode> GetPrevDialoguePosition() = 0;
    virtual TObjectPtr<UDialogueGraphNode> GetCurrentDialoguePosition() = 0;

    virtual void SetPrevDialoguePosition(const TObjectPtr<UDialogueGraphNode>& DialogueGraphNode) = 0;
    virtual void SetCurrentDialoguePosition(const TObjectPtr<UDialogueGraphNode>& DialogueGraphNode) = 0;
};
