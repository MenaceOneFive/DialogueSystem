// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelSequence.h"
#include "UObject/Interface.h"
#include "SectionStack.generated.h"

struct FMovieSceneSequencePlayToParams;
struct FMovieSceneSequencePlaybackParams;
class UMovieSceneCinematicShotSection;


// This class does not need to be modified.
UINTERFACE()
class USectionStack : public UInterface
{
    GENERATED_BODY()
};

/**
 * 
 */
class DIALOGUESYSTEMRUNTIME_API ISectionStack
{
    GENERATED_BODY()

public:
    /// <summary>
    /// SectionStack에 새 레벨 시퀀스를 삽입합니다.
    /// </summary>
    /// <param name="SequenceToPlay">재생할 레벨 시퀀스</param>
    /// <param name="OutPlaybackParams">다음 재생에 사용할 위치 옵션</param>
    /// <param name="OutPlayToParams">재생에 사용할 옵션</param>
    virtual void PushShotSection(ULevelSequence* SequenceToPlay,
                                 FMovieSceneSequencePlaybackParams& OutPlaybackParams,
                                 FMovieSceneSequencePlayToParams& OutPlayToParams) const = 0;

    /// <summary>
    /// 현재 Top에 있는 Section을 제거하고 플레이할 위치를 재조정합니다.
    /// </summary>
    /// <param name="OutPlaybackParams">다음 재생에 사용할 위치 옵션</param>
    /// <param name="OutPlayToParams">재생에 사용할 옵션</param>
    virtual void PopShotSection(FMovieSceneSequencePlaybackParams& OutPlaybackParams,
                                FMovieSceneSequencePlayToParams& OutPlayToParams) const = 0;
};
