// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SequencerSectionBP.h"
#include "DialogueLineSection.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGUESYSTEMRUNTIME_API UDialogueLineSection : public USequencerSectionBP
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    FString CharacterName;

    UPROPERTY(EditAnywhere)
    FString DialogueLine;
};
