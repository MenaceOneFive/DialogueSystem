#pragma once
#include "MovieSceneSequencePlaybackSettings.h"

#include "LevelSequencePlayerPool.generated.h"

class ULevelSequencePlayer;
class ULevelSequence;
class ALevelSequenceActor;
class ULevelSequencePlayerHolder;

/// <summary>
/// ULevelSequencePlayerActor 풀을 관리하는 클래스
/// UAlternativeDialoguePlayerSubsystem에 귀속됨
/// </summary>
UCLASS()
class DIALOGUESYSTEMRUNTIME_API ULevelSequencePlayerInstancePool final : public UObject
{
    GENERATED_BODY()

public:
    void InitializeSequencePlayerPool(const uint32 NumberOfInitialPlayerInstance);
    ULevelSequencePlayerHolder* RequestNewPlayerInstance();

private:
    void CreateLevelSequencePlayerActorInstance();
    void ReturnSequencePlayerActorInstance(ALevelSequenceActor* SequenceActorToReturn);

    [[nodiscard]] FMovieSceneSequencePlaybackSettings GetDefaultPlaybackSetting() const;

    TQueue<ALevelSequenceActor*> SequenceActorPool;

    UPROPERTY()
    ULevelSequence* EmptyLevelSequence;

    friend class ULevelSequencePlayerHolder;
};

/// <summary>
/// LevelSequencePlayerActor의 대여/반납을 담당
/// </summary>
UCLASS()
class DIALOGUESYSTEMRUNTIME_API ULevelSequencePlayerHolder : public UObject
{
    GENERATED_BODY()

public:
    void Initialize(ULevelSequencePlayerInstancePool* PlayerPool);

    virtual void BeginDestroy() override;

    void SetLevelSequencePlayerActor(ALevelSequenceActor* InPlayerActor);

    UFUNCTION(BlueprintCallable)
    ALevelSequenceActor* GetSequencePlayerActor();

    UFUNCTION(BlueprintCallable)
    ULevelSequencePlayer* GetLevelSequencePlayer() const;

    UFUNCTION(BlueprintCallable)
    void SetLevelSequence(ULevelSequence* LevelSequenceToPlay) const;

private:
    UPROPERTY()
    ULevelSequencePlayerInstancePool* ParentPool;

    UPROPERTY()
    ALevelSequenceActor* SequencePlayerActor;
};
