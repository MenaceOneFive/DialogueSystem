// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SequencerTrackBP.h"

#include "DialogueLineTrack.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGUESYSTEMRUNTIME_API UDialogueLineTrack : public USequencerTrackBP
{
    GENERATED_BODY()

public:
    UDialogueLineTrack();
    virtual FName GetTrackName() const override;
#if WITH_EDITORONLY_DATA
    virtual FText GetDisplayName() const override;
#endif
};
