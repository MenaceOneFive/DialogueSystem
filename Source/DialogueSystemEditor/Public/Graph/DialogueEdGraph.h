// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueGraphSchema.h"
#include "EdGraph/EdGraph.h"
#include "DialogueEdGraph.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGUESYSTEMEDITOR_API UDialogueEdGraph : public UEdGraph
{
    GENERATED_BODY()

    UDialogueEdGraph(const FObjectInitializer& Initializer): UEdGraph(Initializer)
    {
        Schema         = UDialogueGraphSchema::StaticClass();
        bEditable      = true;
        bAllowDeletion = true;
        bAllowRenaming = true;
    }
};
