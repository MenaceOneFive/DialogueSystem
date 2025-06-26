// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/DialoguePlayerInstance.h"

#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Component/DialogueUIComponent.h"
#include "GameFramework/Character.h"
#include "Graph/DialogueGraph.h"
#include "Graph/DialogueGraphDirector.h"
#include "Graph/Node/DialogueGraphNode.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/DialoguePlayerSubsystem.h"
#include "Util/SequencerUtil.h"

// constexpr uint32 MinimalFrameEnd = 1;
//
// void UDialoguePlayerInstance::InitializeMasterScene(UMovieScene* MasterScene)
// {
//     // 재생 범위 초기화
//     static_assert(MinimalFrameEnd >= 1, "만약 0으로 설정하면 재생이 되지 않는 문제가 있을 수 있음");
//     MasterScene->SetPlaybackRange(0, MinimalFrameEnd, true);
//     LevelSequencePlayer->SetFrameRange(0, MinimalFrameEnd);
//
//     // Shot 트랙 초기화
//     MasterShotTrack = MasterScene->FindTrack<UMovieSceneCinematicShotTrack>();
//
//     // checkf(MasterShotTrack, TEXT("이 경우 MasterSequence에 UMovieSceneCinematicShotTrack이 없기 때문에 통과하지 못한 것이다."));
//     // -> 너무 빡세게 체크하는듯? 다음으로 변경
//
//     if ( !MasterShotTrack )
//     {
//         MasterShotTrack = MasterScene->AddTrack<UMovieSceneCinematicShotTrack>();
//     }
// }
//
// void UDialoguePlayerInstance::Initialize(ULevelSequence* MasterLevelSequence,
//                                          const UDialogueGraph* InDialogueGraph,
//                                          const FMovieSceneSequencePlaybackSettings& InSettings)
// {
//     DialogueGraph = InDialogueGraph;
//     Settings      = InSettings;
//
//     /// 현 시점에서 코드를 이용해서 레벨 시퀀스를 생성하면 재생이 불가능한 문제가 있음
//     /// 선택 가능한 디자인은 2개가 있음
//     /// 1. 모든 레벨에 레벨 시퀀스를 배치 (90%, 현재 선택한 방법)
//     ///		- 모두 이름이 동일해야 함
//     ///		- 이 함수에서 메모리상에서 에셋을 복제
//     /// 2. 어떻게든 레벨 시퀀스를 생성하는 방법을 찾기 (10%)
//     FObjectDuplicationParameters DupParams{MasterLevelSequence, this};
//     DupParams.ApplyFlags    = RF_Transient;
//     DupParams.DestName      = "TransientMasterLevelSequence";
//     DuplicatedLevelSequence = Cast<ULevelSequence>(StaticDuplicateObjectEx(DupParams));
//     // ULevelSequence* DuplicatedLevelSequence = MasterLevelSequence;
//
//     checkf(DuplicatedLevelSequence, TEXT("에셋 복제에 실패했습니다."));
//
//     // LevelSequencePlayer 초기화
//     LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GEngine->GetCurrentPlayWorld(),
//                                                                           DuplicatedLevelSequence,
//                                                                           Settings,
//                                                                           LevelSequenceActor);
//     check(LevelSequencePlayer && LevelSequenceActor)
//     LevelSequencePlayer->SetPlaybackSettings(Settings);
//
//     DuplicatedLevelSequence->Rename(*DuplicatedLevelSequence->GetName(), LevelSequenceActor, REN_DoNotDirty | REN_DontCreateRedirectors);
//
//     InitializeMasterScene(DuplicatedLevelSequence->GetMovieScene());
//
//     // 재생, 일시정지(노드의 실행이 끝난 경우 발동)에서 사용할 콜백들을 등록
//     LevelSequencePlayer->OnPause.AddDynamic(this, &UDialoguePlayerInstance::HandleNextNode);
//     LevelSequencePlayer->OnStop.AddDynamic(this, &UDialoguePlayerInstance::Release);
//
//     // Visitor를 초기화
//     UDialogueGraphVisitorInstance* VisitorInstance = NewObject<UDialogueGraphVisitorInstance>(this);
//     GraphVisitor                                   = TScriptInterface<IRuntimeDialogueGraphVisitor>(VisitorInstance);
//     GraphVisitor.GetInterface()->InitializeVisitor();
//
//     SetCurrentDialoguePosition(InDialogueGraph->GetStartNode());
// }
//
// void UDialoguePlayerInstance::BeginDestroy()
// {
//     UE_LOG(LogMemory, Display, TEXT("UDialoguePlayerInstance"))
//
//     if ( LevelSequenceActor && LevelSequenceActor->IsValidLowLevelFast() )
//     {
//         LevelSequenceActor->Destroy();
//     }
//     Super::BeginDestroy();
// }
//
// void UDialoguePlayerInstance::Release()
// {
//     if ( !LevelSequencePlayer )
//     {
//         return;
//     }
//
//     if ( GEngine && GEngine->GameViewport )
//     {
//         UGameInstance* GameInstance         = GEngine->GameViewport->GetGameInstance();
//         // UDialoguePlayerSubsystem* Subsystem = GameInstance->GetSubsystem<UDialoguePlayerSubsystem>();
//         // Subsystem->RemoveDialogue();
//     }
//
//     ensureMsgf(LevelSequencePlayer && !LevelSequencePlayer->IsPlaying(),
//                TEXT("재생 중이면 해제하지 말아야함. 만약 재생하고 있는데 Release가 들어오면 문제가 있는 것"));
//
//     // 콜백 제거
//     LevelSequencePlayer->OnPause.RemoveDynamic(this, &UDialoguePlayerInstance::HandleNextNode);
//     LevelSequencePlayer->OnStop.RemoveDynamic(this, &UDialoguePlayerInstance::Release);
//
//     // 다음 재생에 영향을 받지 않도록 기존 자원 해제
//     GraphVisitor.SetInterface(nullptr);
//     GraphVisitor.SetObject(nullptr);
//     MasterShotTrack = nullptr;
//     DialogueGraph   = nullptr;
//
//     PrevDialogueGraphNode    = nullptr;
//     CurrentDialogueGraphNode = nullptr;
//     DuplicatedLevelSequence  = nullptr;
//
//     TArray<UObject*> Referencers;
//
//     GetObjReferenceCount(this, &Referencers);
//
//     for ( UObject* Each : Referencers )
//     {
//         if ( Each )
//         {
//             UE_LOG(LogMemory, Warning, TEXT("%s"), *Each->GetName());
//         }
//     }
// }
//
// [[nodiscard]] FMovieSceneSequencePlaybackSettings UDialoguePlayerInstance::GetDefaultMovieSceneSequencePlaybackSettings()
// {
//     FMovieSceneSequencePlaybackSettings DefaultSettings;
//     DefaultSettings.bAutoPlay   = false;
//     DefaultSettings.bPauseAtEnd = false;
//     return DefaultSettings;
// }
//
// // ReSharper disable once CppMemberFunctionMayBeConst
// // ㄴ const인 경우 AddDynamic에 등록이 불가능함
// void UDialoguePlayerInstance::HandleNextNode()
// {
//     checkf(CurrentDialogueGraphNode,
//            TEXT("CurrentDialogueGraphNode는 Null일 수 없습니다."))
//     // 다형성이 알아서 처리하도록 둔다.
//     CurrentDialogueGraphNode->VisitNode(Cast<IRuntimeDialogueGraphVisitor>(GraphVisitor.GetObject()));
// }
//
//
// [[nodiscard]] UMovieSceneCinematicShotSection* UDialoguePlayerInstance::CreateShotSectionWith(ULevelSequence* SequenceToPlay) const
// {
//     checkf(SequenceToPlay && SequenceToPlay->IsValidLowLevel(),
//            TEXT("SequenceToPlay가 유효하지 않습니다."));
//     // 1단계 섹션 생성
//     UMovieSceneCinematicShotSection* NewShotSection = NewObject<UMovieSceneCinematicShotSection>(MasterShotTrack,
//                                                                                                  UMovieSceneCinematicShotSection::StaticClass(),
//                                                                                                  FName(),
//                                                                                                  RF_Transient);
//     // 2단계 새 섹션에 범위 설정, 시퀀스 설정
//     NewShotSection->SetSequence(SequenceToPlay);
//
//     // Tip:  섹션의 길이를 설정할 때 틱레이트 기준
//     const auto [NewStartTime, NewEndTime] = USequencerUtil::GetQualifiedFrameTimes(SequenceToPlay, ERateType::TickRate);
//
//     // 기본 전제 : 모든 섹션은 처음 생성할 때 0에 위치해야 한다.
//     // 이유     : UDialoguePlayerInstance::MoveSectionToEnd에서 정확한 이동을 위해
//     NewShotSection->SetStartFrame(FFrameNumber{0});
//     NewShotSection->SetEndFrame(NewEndTime.Time.FrameNumber - NewStartTime.Time.FrameNumber);
//     return NewShotSection;
// }
//
// void UDialoguePlayerInstance::MoveSectionToEnd(UMovieSceneCinematicShotSection* NewShotSection) const
// {
//     ULevelSequence* MasterSceneSequence = Cast<ULevelSequence>(LevelSequencePlayer->GetSequence());
//
//     const FFrameNumber NewSectionStartTime = NewShotSection->SectionRange.GetLowerBound().GetValue();
//     checkf(NewSectionStartTime == 0, TEXT("생성된 Section의 시작점은 0으로 정한다."));
//
//     const auto [_, MasterEndTime] = USequencerUtil::GetQualifiedFrameTimes(MasterSceneSequence, ERateType::TickRate);
//
//     NewShotSection->MoveSection(MasterEndTime.Time.FrameNumber);
// }
//
// void UDialoguePlayerInstance::ExpandPlaybacksRange(ULevelSequence* SequenceToPlay) const
// {
//     ULevelSequence* MasterSceneSequence = Cast<ULevelSequence>(LevelSequencePlayer->GetSequence());
//
//     const auto [MasterStartTime, MasterEndTime] = USequencerUtil::GetQualifiedFrameTimes(MasterSceneSequence, ERateType::TickRate);
//     const auto [_, NewSceneEndTime]             = USequencerUtil::GetQualifiedFrameTimes(SequenceToPlay, ERateType::TickRate);
//
//     // Tip : 무비씬의 플레이백 범위 : 틱레이트 기준, 근거 : < Engine\Source\Runtime\MovieScene\Private\MovieScene.cpp:659 >
//     MasterSceneSequence->GetMovieScene()->SetPlaybackRange(MasterStartTime.Time.FrameNumber.Value,
//                                                            MasterEndTime.Time.FrameNumber.Value
//                                                            + NewSceneEndTime.Time.FrameNumber.Value,
//                                                            true);
//
//     // Tip : 플레이어의 범위 : 틱레이트 기준
//     LevelSequencePlayer->SetFrameRange(MasterStartTime.Time.FrameNumber.Value,
//                                        MasterEndTime.Time.FrameNumber.Value
//                                        + NewSceneEndTime.Time.FrameNumber.Value);
// }
//
// void UDialoguePlayerInstance::ShrinkPlaybacksRange(ULevelSequence* RemovedSequence) const
// {
//     ULevelSequence* MasterSceneSequence = Cast<ULevelSequence>(LevelSequencePlayer->GetSequence());
//
//     const auto [MasterStartTime, MasterEndTime]             = USequencerUtil::GetQualifiedFrameTimes(MasterSceneSequence, ERateType::TickRate);
//     const auto [RemovedSceneStartTime, RemovedSceneEndTime] = USequencerUtil::GetQualifiedFrameTimes(RemovedSequence, ERateType::TickRate);
//
//     FFrameNumber RenewedDuration = MasterEndTime.Time.FrameNumber - (RemovedSceneEndTime.Time.FrameNumber - RemovedSceneStartTime.Time.FrameNumber);
//
//     checkf(RenewedDuration.Value >= MinimalFrameEnd, TEXT("길이 계산에 문제가 있습니다."));
//
//     // Tip : 무비씬의 플레이백 범위 : 틱레이트 기준, 근거 : < Engine\Source\Runtime\MovieScene\Private\MovieScene.cpp:659 >
//     MasterSceneSequence->GetMovieScene()->SetPlaybackRange(MasterStartTime.Time.FrameNumber.Value,
//                                                            RenewedDuration.Value,
//                                                            true);
//
//     // Tip : 플레이어의 범위 : 틱레이트 기준
//     LevelSequencePlayer->SetFrameRange(MasterStartTime.Time.FrameNumber.Value, RenewedDuration.Value);
// }
//
// void UDialoguePlayerInstance::UpdatePlaybackPosition(FMovieSceneSequencePlaybackParams& OutPlaybackParams,
//                                                      FMovieSceneSequencePlayToParams& OutPlayToParams) const
// {
//     ULevelSequence* MasterSceneSequence = Cast<ULevelSequence>(LevelSequencePlayer->GetSequence());
//     check(MasterSceneSequence)
//
//     const auto [_, MasterEndTime]  = USequencerUtil::GetQualifiedFrameTimes(MasterSceneSequence, ERateType::DisplayRate);;
//     OutPlaybackParams.PositionType = EMovieScenePositionType::Frame;
//     OutPlaybackParams.Frame        = MasterEndTime.Time.FrameNumber;
//     OutPlaybackParams.bHasJumped   = false;
//     OutPlaybackParams.UpdateMethod = EUpdatePositionMethod::Play;
//
//     OutPlayToParams.bExclusive = true;
// }
//
// #pragma region IDialoguePlayer_Implementation_Pulbic
//
// void UDialoguePlayerInstance::Play() const
// {
//     if ( !Settings.bPauseAtEnd )
//     {
//         UE_LOG(LogTemp, Error, TEXT("UDialoguePlayerInstance::Play() - Settings.bPauseAtEnd is false. PauseAtEnd must be true."));
//         return;
//     }
//     if ( !LevelSequencePlayer )
//     {
//         return;
//     }
//     if ( !LevelSequencePlayer->IsPlaying() )
//     {
//         LevelSequencePlayer->Play();
//     }
// }
//
// void UDialoguePlayerInstance::PlayTo(const FMovieSceneSequencePlaybackParams& PlaybackParams,
//                                      const FMovieSceneSequencePlayToParams& PlayToParams) const
// {
//     if ( !LevelSequencePlayer )
//     {
//         return;
//     }
//     if ( !LevelSequencePlayer->IsPlaying() )
//     {
//         LevelSequencePlayer->PlayTo(PlaybackParams, PlayToParams);
//     }
// }
//
// void UDialoguePlayerInstance::Stop() const
// {
//     if ( !LevelSequencePlayer )
//     {
//         return;
//     }
//
//     if ( LevelSequencePlayer->IsPlaying() || LevelSequencePlayer->IsPaused() )
//     {
//         LevelSequencePlayer->Stop();
//     }
// }
// #pragma endregion
//
// #pragma region ISectionStack_Implementation_Private
// void UDialoguePlayerInstance::PushShotSection(ULevelSequence* SequenceToPlay,
//                                               FMovieSceneSequencePlaybackParams& OutPlaybackParams,
//                                               FMovieSceneSequencePlayToParams& OutPlayToParams) const
// {
//     checkf(SequenceToPlay, TEXT("SequenceToPlay는 null일 수 없습니다."));
//
//     // 1단계 새 섹션을 생성
//     UMovieSceneCinematicShotSection* NewSection = CreateShotSectionWith(SequenceToPlay);
//
//     // 2단계 섹션을 트랙에 추가
//     MasterShotTrack->AddSection(*NewSection);
//
//     // 3단계 섹션을 이동
//     MoveSectionToEnd(NewSection);
//
//     // 4단계 시퀀스 범위 업데이트
//     ExpandPlaybacksRange(SequenceToPlay);
//
//     // 5단계 재생할 범위를 재설정 
//     UpdatePlaybackPosition(OutPlaybackParams, OutPlayToParams);
// }
//
// void UDialoguePlayerInstance::PopShotSection(FMovieSceneSequencePlaybackParams& OutPlaybackParams,
//                                              FMovieSceneSequencePlayToParams& OutPlayToParams) const
// {
//     // 1단계 LIFO를 구현하기 위해 제일 뒤의 요소를 찾기 위해 정렬
//     MasterShotTrack->SortSections();
//     const int32 IndexOfLastSection = MasterShotTrack->GetAllSections().Num() - 1;
//
//     // 2단계 제거 작업을 수행
//     UMovieSceneSequence* EmbeddedSequence = Cast<UMovieSceneCinematicShotSection>(MasterShotTrack->GetAllSections()[IndexOfLastSection])->GetSequence();
//     ULevelSequence* RemovedLevelSequence  = Cast<ULevelSequence>(EmbeddedSequence);
//
//     // 3단계 마스터 시퀀스의 범위를 재설정
//     ShrinkPlaybacksRange(RemovedLevelSequence);
//     MasterShotTrack->RemoveSectionAt(IndexOfLastSection);
//
//     // 4단계 재생할 범위를 재설정 
//     UpdatePlaybackPosition(OutPlaybackParams, OutPlayToParams);
// }
// #pragma endregion
//
// #pragma region IDialoguePositionController_Implementation_Private
// UDialogueGraphNode* UDialoguePlayerInstance::GetPrevDialoguePosition()
// {
//     return PrevDialogueGraphNode;
// }
//
// UDialogueGraphNode* UDialoguePlayerInstance::GetCurrentDialoguePosition()
// {
//     return CurrentDialogueGraphNode;
// }
//
// #pragma endregion
//
// #pragma region IDialoguePositionController_Implementation_Private
//
// void UDialoguePlayerInstance::SetPrevDialoguePosition(UDialogueGraphNode* InDialogueGraphNode)
// {
//     PrevDialogueGraphNode = InDialogueGraphNode;
// }
//
// void UDialoguePlayerInstance::SetCurrentDialoguePosition(UDialogueGraphNode* InDialogueGraphNode)
// {
//     PrevDialogueGraphNode    = CurrentDialogueGraphNode;
//     CurrentDialogueGraphNode = InDialogueGraphNode;
// }
//
// #pragma endregion

class UDialogueGraphDirector;

void UDialoguePlayerInstancePool::InitializeSequencePlayerPool(const uint32 NumberOfInitialPlayerInstance)
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

ULevelSequencePlayerHolder* UDialoguePlayerInstancePool::RequestNewPlayerInstance()
{
    // 풀에 남아 있는 플레이어가 없으면 새로 생성
    if ( SequenceActorPool.IsEmpty() )
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

void UDialoguePlayerInstancePool::CreateLevelSequencePlayerActorInstance()
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

    if ( LevelSequenceActor )
    {
        // LevelSequenceActor->Rename(nullptr, World);
        // 초기화에 성공했으면 Pool에 삽입
        SequenceActorPool.Enqueue(LevelSequenceActor);
    }
}

void UDialoguePlayerInstancePool::ReturnSequencePlayerActorInstance(ALevelSequenceActor* SequenceActorToReturn)
{
    if ( const auto SequencePlayer = SequenceActorToReturn->GetSequencePlayer() )
    {
        SequencePlayer->Stop();
    }
    SequenceActorPool.Enqueue(SequenceActorToReturn);
}

FMovieSceneSequencePlaybackSettings UDialoguePlayerInstancePool::GetDefaultPlaybackSetting() const
{
    FMovieSceneSequencePlaybackSettings DefaultSettings;
    DefaultSettings.bAutoPlay   = false;
    DefaultSettings.bPauseAtEnd = true;
    return DefaultSettings;
}

void ULevelSequencePlayerHolder::Initialize(UDialoguePlayerInstancePool* PlayerPool)
{
    ParentPool = PlayerPool;
}

void ULevelSequencePlayerHolder::BeginDestroy()
{
    UObject::BeginDestroy();

    // 이 홀더에 할당된 자원을 반납
    if ( SequencePlayerActor && SequencePlayerActor->IsValidLowLevel() )
    {
        ParentPool->ReturnSequencePlayerActorInstance(SequencePlayerActor);
    }

    // 연결 끊기
    ParentPool          = nullptr;
    SequencePlayerActor = nullptr;
}

void ULevelSequencePlayerHolder::SetLevelSequencePlayerActor(ALevelSequenceActor* InPlayerActor)
{
    if ( SequencePlayerActor && SequencePlayerActor->IsValidLowLevel() )
    {
    }
    SequencePlayerActor = InPlayerActor;
}

ULevelSequencePlayer* ULevelSequencePlayerHolder::GetLevelSequencePlayer() const
{
    if ( SequencePlayerActor )
    {
        if ( const auto Player = SequencePlayerActor->GetSequencePlayer() )
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

void UDialogueRuntimePlayer::PlayFromBeginning()
{
    if ( !DialogueGraph )
    {
        UE_LOG(LogTemp, Error, TEXT("%s : DialogueGraph is nullptr"), TEXT(__FUNCTION__));
        return;
    }
    if ( const auto StartNode = DialogueGraph->GetStartNode() )
    {
        StartNode->Accept(this);
        return;
    }
    UE_LOG(LogTemp, Error, TEXT("%s : StartNode is nullptr."), TEXT(__FUNCTION__));
}

void UDialogueRuntimePlayer::Pause()
{
}

void UDialogueRuntimePlayer::Stop()
{
}

void UDialogueRuntimePlayer::SetDialogueGraph(const UDialogueGraph* InDialogueGraph)
{
    if ( InDialogueGraph )
    {
        this->DialogueGraph = InDialogueGraph;
        InitializePlayers();
    }

    const auto Class = DialogueGraph->GetBlueprintClass();

    check(Class->IsChildOf(UDialogueGraphDirector::StaticClass()))

    DialogueGraphDirector = NewObject<UDialogueGraphDirector>(this, Class, FName("GraphDirectorInstance"), RF_Transient);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UDialogueRuntimePlayer::VisitNextNode()
{
    // 등록된 콜백들을 모두 제거
    check(CurrentHolder)
    {
        CurrentHolder->GetLevelSequencePlayer()->OnStop.Clear();
        CurrentHolder->GetLevelSequencePlayer()->OnPause.Clear();
        CurrentHolder->GetLevelSequencePlayer()->OnFinished.Clear();
    }

    check(NextNode)
    {
        NextNode->Accept(this);
    }
}

void UDialogueRuntimePlayer::InitializePlayers()
{
    const auto World                                 = GetWorld();
    const UGameInstance* GameInstance                = World->GetGameInstance();
    const UDialoguePlayerRuntimeSubsystem* Subsystem = GameInstance->GetSubsystem<UDialoguePlayerRuntimeSubsystem>();

    for ( const UDialogueGraphNode* DialogueNode : DialogueGraph->GetNodes() )
    {
        if ( const UDialogueSceneNode* SceneNode = Cast<const UDialogueSceneNode>(DialogueNode) )
        {
            const auto LevelSequence = SceneNode->GetLevelSequenceToPlay();
            auto PlayerHolder        = Subsystem->RequestNewPlayerInstance();
            PlayerHolder->Rename(nullptr, this);
            PlayerHolder->SetLevelSequence(LevelSequence);
            SceneNodeToPlayerHolder.Add(SceneNode, PlayerHolder);
        }
    }
}

void UDialogueRuntimePlayer::FocusOnRootWidget() const
{
    const auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    const auto PlayerCharacter  = PlayerController ? PlayerController->GetCharacter() : nullptr;
    const auto Component        = PlayerCharacter ? PlayerCharacter->GetComponentByClass<UDialogueUIComponent>() : nullptr;

    if ( !Component )
    {
        return;
    }

    const auto UIComponent = Cast<UDialogueUIComponent>(Component);
    check(UIComponent)

    const auto RootWidget = UIComponent->GetRootWidget()->GetRootWidget();
    FInputModeUIOnly InputModeUIOnly;
    UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PlayerController, RootWidget, EMouseLockMode::LockAlways);
}

void UDialogueRuntimePlayer::UnfocusFromWidget() const
{
    if ( const auto World = GetWorld(); World->IsGameWorld() )
    {
        if ( const auto PlayerController = UGameplayStatics::GetPlayerController(World, 0) )
        {
            UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
        }
    }
}

void UDialogueRuntimePlayer::ShowCursor() const
{
    if ( const auto World = GetWorld(); World->IsGameWorld() )
    {
        if ( const auto PlayerController = UGameplayStatics::GetPlayerController(World, 0) )
        {
            PlayerController->SetShowMouseCursor(true);
        }
    }
}

void UDialogueRuntimePlayer::HideCursor() const
{
    if ( const auto World = GetWorld(); World->IsGameWorld() )
    {
        if ( const auto PlayerController = UGameplayStatics::GetPlayerController(World, 0) )
        {
            PlayerController->SetShowMouseCursor(false);
        }
    }
}

void UDialogueRuntimePlayer::InitializeVisitor()
{
    NextNode      = DialogueGraph->GetStartNode();
    CurrentHolder = nullptr;
}

void UDialogueRuntimePlayer::VisitStartNode(const UDialogueStartNode* StartNode)
{
    FocusOnRootWidget();
    HideCursor();

    // // 예시1 : 메서드를 호출 (성공, 파라메터와 반환값 둘 다 없는 경우)
    // if ( const auto NewFunction = DirectorClass->FindFunctionByName(FName("NewFunction")) )
    // {
    //     DialogueGraphDirector->ProcessEvent(NewFunction, nullptr);
    // }
    //
    // // 예시2 : 이벤트를 호출 (성공, 파라메터와 반환값 둘 다 없는 경우)
    // if ( const auto CustomEvent = DirectorClass->FindFunctionByName(FName("CustomEvent")) )
    // {
    //     DialogueGraphDirector->ProcessEvent(CustomEvent, nullptr);
    // }
    //
    // // 예시3 : 메서드를 호출 (성공, 파라메터와 반환값이 있는 경우)
    // const auto Function = DirectorClass->FindFunctionByName(FName("NewFunction2"));
    // if ( Function && Function->NumParms > 0 )
    // {
    //     const TSharedRef<FStructOnScope> FuncParams = MakeShared<FStructOnScope>(Function);
    //
    //     // 사용되는 프로퍼티를 정의
    //     const FBoolProperty* Return_BoolValueProperty  = nullptr;
    //     const FStrProperty* Return_StringValueProperty = nullptr;
    //
    //     // 프로퍼티를 유형별로 처리
    //     for ( TFieldIterator<FProperty> It(Function); It; ++It )
    //     {
    //         if ( It->HasAnyPropertyFlags(CPF_Parm) && !It->HasAnyPropertyFlags(CPF_ReturnParm | CPF_OutParm) )
    //         {
    //             if ( const auto ObjectProperty = CastField<FObjectProperty>(*It);
    //                 ObjectProperty
    //                 && ObjectProperty->PropertyClass->IsChildOf<UDialogueGraph>() )
    //             {
    //                 UE_LOG(LogTemp, Log, TEXT(""));
    //             }
    //         }
    //         // 반환 유형인 프로퍼티 ( CPF_OutParam(보통 이쪽임), CPF_ReturnParam )
    //         if ( It->HasAnyPropertyFlags(CPF_ReturnParm | CPF_OutParm) )
    //         {
    //             if ( const auto BoolProperty = CastField<FBoolProperty>(*It)
    //                 ; It->NamePrivate == FName("Result") )
    //             {
    //                 Return_BoolValueProperty = BoolProperty;
    //             }
    //             if ( const auto StringProperty = CastField<FStrProperty>(*It)
    //                 ; It->NamePrivate == FName("StrStr") )
    //             {
    //                 Return_StringValueProperty = StringProperty;
    //             }
    //             continue;
    //         }
    //         if ( It->HasAnyPropertyFlags(CPF_Parm) )
    //         {
    //             continue;
    //         }
    //         if ( It->HasAnyPropertyFlags(CPF_ReferenceParm) )
    //         {
    //             continue;
    //         }
    //     }
    //
    //     // 프로퍼티를 이용해서 파라메터를 초기화
    //
    //     // 블루프린트 메서드를 호출
    //     DialogueGraphDirector->ProcessEvent(Function, FuncParams->GetStructMemory());
    //
    //     // 프로퍼티를 이용해서 결과를 추출
    //     auto BoolReturnValue   = Return_BoolValueProperty ? Return_BoolValueProperty->GetPropertyValue_InContainer(FuncParams->GetStructMemory()) : false;
    //     auto StringReturnValue = Return_StringValueProperty ? Return_StringValueProperty->GetPropertyValue_InContainer(FuncParams->GetStructMemory()) : "";
    //     UE_LOG(LogTemp, Log, TEXT(""));
    // }

    // 방문을 통해 발생하는 효과 처리
    WhenVisitThisNode(PrevNode, StartNode);

    // 다음 노드 설정
    NextNode = DialogueGraph->GetNode(StartNode->GetNextNodeID());

    // 이전 노드 설정
    PrevNode = StartNode;

    NextNode->Accept(this);

    // if ( const auto ConditionalNode = Cast<UDialogueConditionalNode>(NextNode) )
    // {
    //     if ( const auto FunctionName = ConditionalNode->GetCanSelectThisNodeFunctionName(); !FunctionName.IsEqual(NAME_None) )
    //     {
    //         const auto Function = DirectorClass->FindFunctionByName(FunctionName);
    //         check(Function)
    //         if ( DialogueGraphDirector->CanVisitNode(Function, NextNode) )
    //         {
    //             NextNode->Accept(this);
    //         }
    //     }
    //     else
    //     {
    //         NextNode->Accept(this);
    //     }
    // }
    // else
    // {
    //     NextNode->Accept(this);
    // }
}

void UDialogueRuntimePlayer::VisitSelectionNode(const UDialogueSelectionNode* SelectionNode)
{
    // 방문을 통해 발생하는 효과 처리
    WhenVisitThisNode(PrevNode, SelectionNode);

    // 선택지 초기화
    const auto NextNodeIds      = SelectionNode->GetNextNodeIDs();
    const auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    const auto PlayerCharacter  = PlayerController ? PlayerController->GetCharacter() : nullptr;
    const auto Component        = PlayerCharacter ? PlayerCharacter->GetComponentByClass<UDialogueUIComponent>() : nullptr;

    if ( !Component )
    {
        return;
    }

    const auto UIComponent = Cast<UDialogueUIComponent>(Component);
    check(UIComponent)

    PrevNode = SelectionNode;

    TArray<UDialogueSelectionItem*> SelectionItem = SelectionNode->GetSelectionItems();
    for ( UDialogueSelectionItem* DialogueSelectionItem : SelectionItem )
    {
        if ( !DialogueSelectionItem->IsValidLowLevel() )
        {
            continue;
        }
        DialogueSelectionItem->InitializeForExecution(DialogueGraphDirector, this);
    }

    UIComponent->SetSelectionItem(SelectionItem);
    UIComponent->ShowSelectionUI();
    FocusOnRootWidget();
    ShowCursor();
}

void UDialogueRuntimePlayer::VisitSceneNode(const UDialogueSceneNode* SceneNode)
{
    // 방문을 통해 발생하는 효과 처리
    WhenVisitThisNode(PrevNode, SceneNode);

    // 현재 노드를 이용해서 재생을 준비
    HideCursor();
    CurrentHolder = SceneNodeToPlayerHolder[SceneNode];

    NextNode = DialogueGraph->GetNode(SceneNode->GetNextNodeID());

    CurrentHolder->GetLevelSequencePlayer()->OnStop.AddDynamic(this, &UDialogueRuntimePlayer::VisitNextNode);
    CurrentHolder->GetLevelSequencePlayer()->OnPause.AddDynamic(this, &UDialogueRuntimePlayer::VisitNextNode);
    CurrentHolder->GetLevelSequencePlayer()->OnFinished.AddDynamic(this, &UDialogueRuntimePlayer::VisitNextNode);

    const auto Player = CurrentHolder->GetLevelSequencePlayer();

    PrevNode = SceneNode;

    // 다시 재생할 수 있으니 재생위치 초기화
    Player->SetPlaybackPosition({0, EUpdatePositionMethod::Jump/** 재생모드가 확실하지 않아서 실험 필요 */});
    Player->Play();
}

void UDialogueRuntimePlayer::VisitEndNode(const UDialogueEndNode* EndNode)
{
    // 방문을 통해 발생하는 효과 처리
    WhenVisitThisNode(PrevNode, EndNode);

    // 리소스 정리
    UnfocusFromWidget();
    HideCursor();

    TArray<ULevelSequencePlayerHolder*> OutArray;
    SceneNodeToPlayerHolder.GenerateValueArray(OutArray);
    for ( const auto LevelSequencePlayerHolder : OutArray )
    {
        const auto Player = LevelSequencePlayerHolder->GetLevelSequencePlayer();
        Player->OnStop.Clear();
        Player->OnPause.Clear();
        Player->OnFinished.Clear();
        Player->Stop();
        CurrentHolder = nullptr;
        NextNode      = nullptr;
        PrevNode      = nullptr;
    }

    if ( const auto World = GetWorld() )
    {
        FTimerHandle TimeHandle;
        World->GetTimerManager().SetTimer(TimeHandle,
                                          this,
                                          &UDialogueRuntimePlayer::HideSubtitle,
                                          3.0f, false
                                         );
    }
}

void UDialogueRuntimePlayer::VisitEventNode(const UDialogueEventNode* EventNode)
{
    // 방문을 통해 발생하는 효과 처리
    WhenVisitThisNode(PrevNode, EventNode);

    HideCursor();

    NextNode = DialogueGraph->GetNode(EventNode->GetNextNodeID());
    PrevNode = EventNode;

    NextNode->Accept(this);
}

void UDialogueRuntimePlayer::WhenVisitThisNode(const UDialogueGraphNode* InPrevNode,
                                               const UDialogueGraphNode* InCurrentNode) const
{
    // 방문을 통해 발생하는 효과 처리
    const auto DirectorClass = DialogueGraph->GetBlueprintClass();

    if ( const auto WhenVisitThisNodeName = InCurrentNode->GetWhenSelectThisNodeFunctionName(); !WhenVisitThisNodeName.IsEqual(NAME_None) )
    {
        const auto Function = DirectorClass->FindFunctionByName(WhenVisitThisNodeName);
        check(Function)
        DialogueGraphDirector->WhenVisitThisNode(Function, InPrevNode, InCurrentNode);
    }
}

void UDialogueRuntimePlayer::HideSubtitle() const
{
    const auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    const auto PlayerCharacter  = PlayerController ? PlayerController->GetCharacter() : nullptr;
    const auto Component        = PlayerCharacter ? PlayerCharacter->GetComponentByClass<UDialogueUIComponent>() : nullptr;

    if ( !Component )
    {
        return;
    }

    const auto UIComponent = Cast<UDialogueUIComponent>(Component);
    check(UIComponent)
    UIComponent->HideSubtitle();
}
