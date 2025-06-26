// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/BlueprintExtension.h"
#include "DialogueGraphBlueprintExtension.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGUESYSTEMEDITOR_API UDialogueGraphBlueprintExtension : public UBlueprintExtension
{
    GENERATED_BODY()
    virtual void HandlePreloadObjectsForCompilation(UBlueprint* OwningBlueprint) override;
    virtual void HandleGenerateFunctionGraphs(FKismetCompilerContext* CompilerContext) override;
};
