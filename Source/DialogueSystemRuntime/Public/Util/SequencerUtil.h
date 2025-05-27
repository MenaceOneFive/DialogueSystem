// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovieScene.h"
#include "MovieSceneSequence.h"
#include "MovieSceneTimeHelpers.h"
#include "SequencerUtil.generated.h"

UENUM(BlueprintType)
enum class ERateType:uint8 { TickRate, DisplayRate };

USTRUCT(BlueprintType)
struct FQualifiedFrameTimes
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FQualifiedFrameTime StartTime;

    UPROPERTY(BlueprintReadWrite)
    FQualifiedFrameTime EndTime;
};

/**
 * 참고할 자료 : https://dev.epicgames.com/documentation/ko-kr/unreal-engine/sequencer-time-refactor-technical-notes?application_version=4.27
 */
UCLASS()
class DIALOGUESYSTEMRUNTIME_API USequencerUtil final : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * 시퀀스에서 디스플레이 레이트를 기준으로 시작 프레임과 끝 프레임의 프레임타임을 구한다.
     * @param MovieSceneSequence 전체 길이를 알고 싶은 무비 시퀀스
     * @return 디스플레이 레이트 기준의 양끝단 프레임타임(틱레이트로 계산했을 때보다 매우 작은) 정보
     */
    UFUNCTION(BlueprintCallable)
    static FQualifiedFrameTimes GetQualifiedFrameTimesWithDisplayRate(UMovieSceneSequence* MovieSceneSequence)
    {
        if (!ensure(MovieSceneSequence))
        {
            return FQualifiedFrameTimes{};
        }
        const UMovieScene* MovieScene = MovieSceneSequence->GetMovieScene();
        TRange<FFrameNumber> PlaybackRange = MovieScene->GetPlaybackRange();

        const FFrameRate TickResolution = MovieScene->GetTickResolution();
        const FFrameRate DisplayRate = MovieScene->GetDisplayRate();

        using namespace UE::MovieScene;
        const FFrameTime StartingTime = ConvertFrameTime(DiscreteInclusiveLower(PlaybackRange),
                                                         TickResolution,
                                                         DisplayRate);
        const FFrameTime EndingTime = ConvertFrameTime(DiscreteExclusiveUpper(PlaybackRange),
                                                       TickResolution,
                                                       DisplayRate);

        FQualifiedFrameTime QualifiedFrameTime{StartingTime, DisplayRate};
        FQualifiedFrameTime FrameTime{EndingTime, DisplayRate};
        return {QualifiedFrameTime, FrameTime};
    }

    /**
     * 시퀀스에서 틱-레이트를 기준으로 시작 프레임과 끝 프레임의 프레임타임을 구한다.
     * @param MovieSceneSequence 전체 길이를 알고 싶은 무비 시퀀스
     * @return 틱레이트 기준의 양끝단 프레임타임(엄청 큰 값) 정보
     */
    UFUNCTION(BlueprintCallable)
    static FQualifiedFrameTimes GetQualifiedFrameTimesWithTickRate(UMovieSceneSequence* MovieSceneSequence)
    {
        if (!ensure(MovieSceneSequence))
        {
            return FQualifiedFrameTimes{};
        }
        const UMovieScene* MovieScene = MovieSceneSequence->GetMovieScene();
        TRange<FFrameNumber> PlaybackRange = MovieScene->GetPlaybackRange();

        FFrameRate TickResolution = MovieScene->GetTickResolution();

        using namespace UE::MovieScene;
        FQualifiedFrameTime QualifiedFrameTime{
            (DiscreteInclusiveLower(PlaybackRange)),
            TickResolution
        };
        FQualifiedFrameTime FrameTime{
            (DiscreteExclusiveUpper(PlaybackRange)),
            TickResolution
        };
        return {QualifiedFrameTime, FrameTime};
    }


    /**
     * 시퀀스에서 틱-레이트를 기준으로 시작 프레임과 끝 프레임의 프레임타임을 구한다.
     * @param MovieSceneSequence 전체 길이를 알고 싶은 무비 시퀀스
     * @param Rate 틱 / 디스플레이 중 사용할 기준
     * @return 틱 / 디스플레이 레이트 기준의 양끝단 프레임타임 정보
     */
    UFUNCTION(BlueprintCallable)
    static FQualifiedFrameTimes GetQualifiedFrameTimes(UMovieSceneSequence* MovieSceneSequence, const ERateType Rate)
    {
        switch (Rate)
        {
        case ERateType::DisplayRate:
            return GetQualifiedFrameTimesWithDisplayRate(MovieSceneSequence);
        case ERateType::TickRate:
            return GetQualifiedFrameTimesWithTickRate(MovieSceneSequence);
        default:
            return FQualifiedFrameTimes{};
        }
    }
};
