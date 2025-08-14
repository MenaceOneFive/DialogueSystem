// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DialoguePlayer.generated.h"

struct FMovieSceneSequencePlayToParams;
struct FMovieSceneSequencePlaybackParams;

// This class does not need to be modified.
UINTERFACE()
class UDialoguePlayer : public UInterface
{
    GENERATED_BODY()
};

/**
 * 
 */
class DIALOGUESYSTEMRUNTIME_API IDialoguePlayer
{
    GENERATED_BODY()

public:
    // 대화를 재생합니다.
    virtual void Play() const = 0;

    // 특정 위치로 이동해서 재생합니다.
    virtual void PlayTo(const FMovieSceneSequencePlaybackParams& PlaybackParams,
                        const FMovieSceneSequencePlayToParams& PlayToParams) const = 0;

    // 대화를 중단합니다.
    virtual void Stop() const = 0;
};
