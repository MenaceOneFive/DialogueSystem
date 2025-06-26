// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SequencerTrackInstanceBP.h"
#include "DialogueLineTrackInstance.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGUESYSTEMRUNTIME_API UDialogueLineTrackInstance : public USequencerTrackInstanceBP
{
    GENERATED_BODY()

public:
    UDialogueLineTrackInstance();

private:
    /// <summary>
    /// 현재 섹션에 해당하는 자막을 표시합니다.
    /// </summary>
    /// <param name="InInput"></param>
    virtual void OnInputAdded(const FMovieSceneTrackInstanceInput& InInput) override;

    /// <summary>
    /// 현재 섹션에 해당하는 자막을 제거합니다.
    /// </summary>
    /// <param name="InInput"></param>
    virtual void OnInputRemoved(const FMovieSceneTrackInstanceInput& InInput) override;
};
