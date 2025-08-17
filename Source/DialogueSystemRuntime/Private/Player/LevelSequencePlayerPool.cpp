#include "Player/LevelSequencePlayerPool.h"

#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "MovieScene.h"
#include "MovieSceneSequencePlaybackSettings.h"

void ULevelSequencePlayerInstancePool::InitializeSequencePlayerPool(const uint32 NumberOfInitialPlayerInstance)
{
    // 빈 Dummy LevelSequence를 생성 : CreateLevelSequencePlayer에 필요해서
    EmptyLevelSequence             = NewObject<ULevelSequence>(this);
    EmptyLevelSequence->MovieScene = NewObject<UMovieScene>(EmptyLevelSequence);
    EmptyLevelSequence->MovieScene->SetPlaybackRange(0, 1, true);

    // 지정된 수 만큼 Actor들을 생성
    // for ( uint32 i = 0; i < NumberOfInitialPlayerInstance; ++i )
    // {
    //     CreateLevelSequencePlayerActorInstance();
    // }
}

ULevelSequencePlayerHolder* ULevelSequencePlayerInstancePool::RequestNewPlayerInstance()
{
    // 풀에 남아 있는 플레이어가 없으면 새로 생성
    if (SequenceActorPool.IsEmpty())
    {
        CreateLevelSequencePlayerActorInstance();
        check(!SequenceActorPool.IsEmpty())
    }

    ALevelSequenceActor* OutPlayer = *SequenceActorPool.Peek();
    SequenceActorPool.Pop();

    check(this->IsValidLowLevel())

    check(OutPlayer->IsValidLowLevel())

    // Holder를 초기화
    const auto Holder = NewObject<ULevelSequencePlayerHolder>(this);
    Holder->Initialize(this);
    Holder->SetLevelSequencePlayerActor(OutPlayer);

    return Holder;
}

void ULevelSequencePlayerInstancePool::CreateLevelSequencePlayerActorInstance()
{
    const FMovieSceneSequencePlaybackSettings DefaultSettings = GetDefaultPlaybackSetting();
    ALevelSequenceActor* LevelSequenceActor;
    const auto World = GetWorld();

    checkf(World->IsGameWorld(),
           TEXT("게임 월드가 아닌 곳에서 이 메서드를 호출했고 실제로 이 액터에 접근하면 크래시가 발생합니다."))

    ULevelSequencePlayer::CreateLevelSequencePlayer(World
                                                    , EmptyLevelSequence
                                                    , DefaultSettings
                                                    , LevelSequenceActor);

    if (LevelSequenceActor)
    {
        // LevelSequenceActor->Rename(nullptr, World);
        // 초기화에 성공했으면 Pool에 삽입
        SequenceActorPool.Enqueue(LevelSequenceActor);
    }
}

void ULevelSequencePlayerInstancePool::ReturnSequencePlayerActorInstance(ALevelSequenceActor* SequenceActorToReturn)
{
    if (const auto SequencePlayer = SequenceActorToReturn->GetSequencePlayer())
    {
        SequencePlayer->Stop();
    }
    SequenceActorPool.Enqueue(SequenceActorToReturn);
}

FMovieSceneSequencePlaybackSettings ULevelSequencePlayerInstancePool::GetDefaultPlaybackSetting() const
{
    FMovieSceneSequencePlaybackSettings DefaultSettings;
    DefaultSettings.bAutoPlay   = false;
    DefaultSettings.bPauseAtEnd = true;
    return DefaultSettings;
}

void ULevelSequencePlayerHolder::Initialize(ULevelSequencePlayerInstancePool* PlayerPool)
{
    ParentPool = PlayerPool;
}

void ULevelSequencePlayerHolder::BeginDestroy()
{
    UObject::BeginDestroy();

    // 이 홀더에 할당된 자원을 반납
    if (SequencePlayerActor && SequencePlayerActor->IsValidLowLevel())
    {
        ParentPool->ReturnSequencePlayerActorInstance(SequencePlayerActor);
    }

    // 연결 끊기
    ParentPool          = nullptr;
    SequencePlayerActor = nullptr;
}

void ULevelSequencePlayerHolder::SetLevelSequencePlayerActor(ALevelSequenceActor* InPlayerActor)
{
    if (SequencePlayerActor && SequencePlayerActor->IsValidLowLevel())
    {
    }
    SequencePlayerActor = InPlayerActor;
}

ALevelSequenceActor* ULevelSequencePlayerHolder::GetSequencePlayerActor()
{
    return SequencePlayerActor;
}

ULevelSequencePlayer* ULevelSequencePlayerHolder::GetLevelSequencePlayer() const
{
    if (SequencePlayerActor)
    {
        if (const auto Player = SequencePlayerActor->GetSequencePlayer())
        {
            return Player;
        }
    }
    return nullptr;
}

void ULevelSequencePlayerHolder::SetLevelSequence(ULevelSequence* LevelSequenceToPlay) const
{
    // IsPlaying을 false로 만들기
    auto LevelSequencePlayer = GetLevelSequencePlayer();
    LevelSequencePlayer->Pause();
    SequencePlayerActor->SetSequence(LevelSequenceToPlay);
}
