// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/DialoguePlayerInstance.h"

#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "DataStructure/Graph/DialogueGraph.h"
#include "DataStructure/Graph/DialogueNode.h"
#include "Sections/MovieSceneCinematicShotSection.h"
#include "Subsystem/DialoguePlayerSubsystem.h"
#include "Tracks/MovieSceneCinematicShotTrack.h"
#include "Util/MemoryUtils.h"
#include "Util/SequencerUtil.h"

constexpr uint32 MinimalFrameEnd = 1;

void UDialoguePlayerInstance::InitializeMasterScene(UMovieScene* MasterScene)
{
    // 재생 범위 초기화
    static_assert(MinimalFrameEnd >= 1, "만약 0으로 설정하면 재생이 되지 않는 문제가 있을 수 있음");
    MasterScene->SetPlaybackRange(0, MinimalFrameEnd, true);
    LevelSequencePlayer->SetFrameRange(0, MinimalFrameEnd);

    // Shot 트랙 초기화
    MasterShotTrack = MasterScene->FindTrack<UMovieSceneCinematicShotTrack>();

    // checkf(MasterShotTrack, TEXT("이 경우 MasterSequence에 UMovieSceneCinematicShotTrack이 없기 때문에 통과하지 못한 것이다."));
    // -> 너무 빡세게 체크하는듯? 다음으로 변경

    if (!MasterShotTrack)
    {
        MasterShotTrack = MasterScene->AddTrack<UMovieSceneCinematicShotTrack>();
    }
}

void UDialoguePlayerInstance::Initialize(ULevelSequence* MasterLevelSequence,
                                         const UDialogueGraph* InDialogueGraph,
                                         const FMovieSceneSequencePlaybackSettings& InSettings)
{
    DialogueGraph = InDialogueGraph;
    Settings      = InSettings;

    /// 현 시점에서 코드를 이용해서 레벨 시퀀스를 생성하면 재생이 불가능한 문제가 있음
    /// 선택 가능한 디자인은 2개가 있음
    /// 1. 모든 레벨에 레벨 시퀀스를 배치 (90%, 현재 선택한 방법)
    ///		- 모두 이름이 동일해야 함
    ///		- 이 함수에서 메모리상에서 에셋을 복제
    /// 2. 어떻게든 레벨 시퀀스를 생성하는 방법을 찾기 (10%)
    FObjectDuplicationParameters DupParams{MasterLevelSequence, this};
    DupParams.ApplyFlags    = RF_Transient;
    DupParams.DestName      = "TransientMasterLevelSequence";
    DuplicatedLevelSequence = Cast<ULevelSequence>(StaticDuplicateObjectEx(DupParams));
    // ULevelSequence* DuplicatedLevelSequence = MasterLevelSequence;

    checkf(DuplicatedLevelSequence, TEXT("에셋 복제에 실패했습니다."));

    // LevelSequencePlayer 초기화
    LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GEngine->GetCurrentPlayWorld(), DuplicatedLevelSequence, Settings, LevelSequenceActor);
    check(LevelSequencePlayer && LevelSequenceActor)
    LevelSequencePlayer->SetPlaybackSettings(Settings);

    DuplicatedLevelSequence->Rename(*DuplicatedLevelSequence->GetName(), LevelSequenceActor, REN_DoNotDirty | REN_DontCreateRedirectors);

    InitializeMasterScene(DuplicatedLevelSequence->GetMovieScene());

    // 재생, 일시정지(노드의 실행이 끝난 경우 발동)에서 사용할 콜백들을 등록
    LevelSequencePlayer->OnPause.AddDynamic(this, &UDialoguePlayerInstance::HandleNextNode);
    LevelSequencePlayer->OnStop.AddDynamic(this, &UDialoguePlayerInstance::Release);

    // Visitor를 초기화
    UDialogueGraphVisitorInstance* VisitorInstance = NewObject<UDialogueGraphVisitorInstance>(this);
    GraphVisitor                                   = TScriptInterface<IDialogueGraphVisitor>(VisitorInstance);
    GraphVisitor.GetInterface()->Initialize();

    SetCurrentDialoguePosition(InDialogueGraph->GetStartNode());
}

void UDialoguePlayerInstance::BeginDestroy()
{
    UE_LOG(LogMemory, Display, TEXT("UDialoguePlayerInstance"))

    if (LevelSequenceActor && LevelSequenceActor->IsValidLowLevelFast())
    {
        LevelSequenceActor->Destroy();
    }
    Super::BeginDestroy();
}

void UDialoguePlayerInstance::Release()
{
    if (!LevelSequencePlayer)
    {
        return;
    }

    if (GEngine && GEngine->GameViewport)
    {
        UGameInstance* GameInstance         = GEngine->GameViewport->GetGameInstance();
        UDialoguePlayerSubsystem* Subsystem = GameInstance->GetSubsystem<UDialoguePlayerSubsystem>();
        Subsystem->RemoveDialogue();
    }

    ensureMsgf(LevelSequencePlayer && !LevelSequencePlayer->IsPlaying(),
               TEXT("재생 중이면 해제하지 말아야함. 만약 재생하고 있는데 Release가 들어오면 문제가 있는 것"));

    // 콜백 제거
    LevelSequencePlayer->OnPause.RemoveDynamic(this, &UDialoguePlayerInstance::HandleNextNode);
    LevelSequencePlayer->OnStop.RemoveDynamic(this, &UDialoguePlayerInstance::Release);

    // 다음 재생에 영향을 받지 않도록 기존 자원 해제
    GraphVisitor.SetInterface(nullptr);
    GraphVisitor.SetObject(nullptr);
    MasterShotTrack = nullptr;
    DialogueGraph   = nullptr;

    PrevDialogueGraphNode    = nullptr;
    CurrentDialogueGraphNode = nullptr;
    DuplicatedLevelSequence  = nullptr;

    TArray<UObject*> Referencers;

    GetObjReferenceCount(this, &Referencers);

    for (UObject* Each : Referencers)
    {
        if (Each)
        {
            UE_LOG(LogMemory, Warning, TEXT("%s"), *Each->GetName());
        }
    }
}

[[nodiscard]] FMovieSceneSequencePlaybackSettings UDialoguePlayerInstance::GetDefaultMovieSceneSequencePlaybackSettings()
{
    FMovieSceneSequencePlaybackSettings DefaultSettings;
    DefaultSettings.bAutoPlay   = false;
    DefaultSettings.bPauseAtEnd = true;
    return DefaultSettings;
}

// ReSharper disable once CppMemberFunctionMayBeConst
// ㄴ const인 경우 AddDynamic에 등록이 불가능함
void UDialoguePlayerInstance::HandleNextNode()
{
    checkf(CurrentDialogueGraphNode,
           TEXT("CurrentDialogueGraphNode는 Null일 수 없습니다."))
    // 다형성이 알아서 처리하도록 둔다.
    CurrentDialogueGraphNode->VisitNode(Cast<IDialogueGraphVisitor>(GraphVisitor.GetObject()));
}


[[nodiscard]] UMovieSceneCinematicShotSection* UDialoguePlayerInstance::CreateShotSectionWith(ULevelSequence* SequenceToPlay) const
{
    checkf(SequenceToPlay && SequenceToPlay->IsValidLowLevel(),
           TEXT("SequenceToPlay가 유효하지 않습니다."));
    // 1단계 섹션 생성
    UMovieSceneCinematicShotSection* NewShotSection = NewObject<UMovieSceneCinematicShotSection>(MasterShotTrack,
                                                                                                 UMovieSceneCinematicShotSection::StaticClass(),
                                                                                                 FName(),
                                                                                                 RF_Transient);
    // 2단계 새 섹션에 범위 설정, 시퀀스 설정
    NewShotSection->SetSequence(SequenceToPlay);

    // Tip:  섹션의 길이를 설정할 때 틱레이트 기준
    const auto [NewStartTime, NewEndTime] = USequencerUtil::GetQualifiedFrameTimes(SequenceToPlay, ERateType::TickRate);

    // 기본 전제 : 모든 섹션은 처음 생성할 때 0에 위치해야 한다.
    // 이유     : UDialoguePlayerInstance::MoveSectionToEnd에서 정확한 이동을 위해
    NewShotSection->SetStartFrame(FFrameNumber{0});
    NewShotSection->SetEndFrame(NewEndTime.Time.FrameNumber - NewStartTime.Time.FrameNumber);
    return NewShotSection;
}

void UDialoguePlayerInstance::MoveSectionToEnd(UMovieSceneCinematicShotSection* NewShotSection) const
{
    ULevelSequence* MasterSceneSequence = Cast<ULevelSequence>(LevelSequencePlayer->GetSequence());

    const FFrameNumber NewSectionStartTime = NewShotSection->SectionRange.GetLowerBound().GetValue();
    checkf(NewSectionStartTime == 0, TEXT("생성된 Section의 시작점은 0으로 정한다."));

    const auto [_, MasterEndTime] = USequencerUtil::GetQualifiedFrameTimes(MasterSceneSequence, ERateType::TickRate);

    NewShotSection->MoveSection(MasterEndTime.Time.FrameNumber);
}

void UDialoguePlayerInstance::ExpandPlaybacksRange(ULevelSequence* SequenceToPlay) const
{
    ULevelSequence* MasterSceneSequence = Cast<ULevelSequence>(LevelSequencePlayer->GetSequence());

    const auto [MasterStartTime, MasterEndTime] = USequencerUtil::GetQualifiedFrameTimes(MasterSceneSequence, ERateType::TickRate);
    const auto [_, NewSceneEndTime]             = USequencerUtil::GetQualifiedFrameTimes(SequenceToPlay, ERateType::TickRate);

    // Tip : 무비씬의 플레이백 범위 : 틱레이트 기준, 근거 : < Engine\Source\Runtime\MovieScene\Private\MovieScene.cpp:659 >
    MasterSceneSequence->GetMovieScene()->SetPlaybackRange(MasterStartTime.Time.FrameNumber.Value,
                                                           MasterEndTime.Time.FrameNumber.Value
                                                           + NewSceneEndTime.Time.FrameNumber.Value,
                                                           true);

    // Tip : 플레이어의 범위 : 틱레이트 기준
    LevelSequencePlayer->SetFrameRange(MasterStartTime.Time.FrameNumber.Value,
                                       MasterEndTime.Time.FrameNumber.Value
                                       + NewSceneEndTime.Time.FrameNumber.Value);
}

void UDialoguePlayerInstance::ShrinkPlaybacksRange(ULevelSequence* RemovedSequence) const
{
    ULevelSequence* MasterSceneSequence = Cast<ULevelSequence>(LevelSequencePlayer->GetSequence());

    const auto [MasterStartTime, MasterEndTime]             = USequencerUtil::GetQualifiedFrameTimes(MasterSceneSequence, ERateType::TickRate);
    const auto [RemovedSceneStartTime, RemovedSceneEndTime] = USequencerUtil::GetQualifiedFrameTimes(RemovedSequence, ERateType::TickRate);

    FFrameNumber RenewedDuration = MasterEndTime.Time.FrameNumber - (RemovedSceneEndTime.Time.FrameNumber - RemovedSceneStartTime.Time.FrameNumber);

    checkf(RenewedDuration.Value >= MinimalFrameEnd, TEXT("길이 계산에 문제가 있습니다."));

    // Tip : 무비씬의 플레이백 범위 : 틱레이트 기준, 근거 : < Engine\Source\Runtime\MovieScene\Private\MovieScene.cpp:659 >
    MasterSceneSequence->GetMovieScene()->SetPlaybackRange(MasterStartTime.Time.FrameNumber.Value,
                                                           RenewedDuration.Value,
                                                           true);

    // Tip : 플레이어의 범위 : 틱레이트 기준
    LevelSequencePlayer->SetFrameRange(MasterStartTime.Time.FrameNumber.Value, RenewedDuration.Value);
}

void UDialoguePlayerInstance::UpdatePlaybackPosition(FMovieSceneSequencePlaybackParams& OutPlaybackParams,
                                                     FMovieSceneSequencePlayToParams& OutPlayToParams) const
{
    ULevelSequence* MasterSceneSequence = Cast<ULevelSequence>(LevelSequencePlayer->GetSequence());
    check(MasterSceneSequence)

    const auto [_, MasterEndTime]  = USequencerUtil::GetQualifiedFrameTimes(MasterSceneSequence, ERateType::DisplayRate);;
    OutPlaybackParams.PositionType = EMovieScenePositionType::Frame;
    OutPlaybackParams.Frame        = MasterEndTime.Time.FrameNumber;
    OutPlaybackParams.bHasJumped   = false;
    OutPlaybackParams.UpdateMethod = EUpdatePositionMethod::Play;

    OutPlayToParams.bExclusive = true;
}

#pragma region IDialoguePlayer_Implementation_Pulbic

void UDialoguePlayerInstance::Play() const
{
    check(Settings.bPauseAtEnd) // 이 값은 무조건 True여야 한다. 만약 False이면 OnPause가 동작하지 않아 그래프 순회가 불가능해진다.
    if (!LevelSequencePlayer)
    {
        return;
    }
    if (!LevelSequencePlayer->IsPlaying())
    {
        LevelSequencePlayer->Play();
    }
}

void UDialoguePlayerInstance::PlayTo(const FMovieSceneSequencePlaybackParams& PlaybackParams,
                                     const FMovieSceneSequencePlayToParams& PlayToParams) const
{
    if (!LevelSequencePlayer)
    {
        return;
    }
    if (!LevelSequencePlayer->IsPlaying())
    {
        LevelSequencePlayer->PlayTo(PlaybackParams, PlayToParams);
    }
}

void UDialoguePlayerInstance::Stop() const
{
    if (!LevelSequencePlayer)
    {
        return;
    }

    if (LevelSequencePlayer->IsPlaying() || LevelSequencePlayer->IsPaused())
    {
        LevelSequencePlayer->Stop();
    }
}
#pragma endregion

#pragma region ISectionStack_Implementation_Private
void UDialoguePlayerInstance::PushShotSection(ULevelSequence* SequenceToPlay,
                                              FMovieSceneSequencePlaybackParams& OutPlaybackParams,
                                              FMovieSceneSequencePlayToParams& OutPlayToParams) const
{
    checkf(SequenceToPlay, TEXT("SequenceToPlay는 null일 수 없습니다."));

    // 1단계 새 섹션을 생성
    UMovieSceneCinematicShotSection* NewSection = CreateShotSectionWith(SequenceToPlay);

    // 2단계 섹션을 트랙에 추가
    MasterShotTrack->AddSection(*NewSection);

    // 3단계 섹션을 이동
    MoveSectionToEnd(NewSection);

    // 4단계 시퀀스 범위 업데이트
    ExpandPlaybacksRange(SequenceToPlay);

    // 5단계 재생할 범위를 재설정 
    UpdatePlaybackPosition(OutPlaybackParams, OutPlayToParams);
}

void UDialoguePlayerInstance::PopShotSection(FMovieSceneSequencePlaybackParams& OutPlaybackParams,
                                             FMovieSceneSequencePlayToParams& OutPlayToParams) const
{
    // 1단계 LIFO를 구현하기 위해 제일 뒤의 요소를 찾기 위해 정렬
    MasterShotTrack->SortSections();
    int32 IndexOfLastSection = MasterShotTrack->GetAllSections().Num() - 1;

    // 2단계 제거 작업을 수행
    UMovieSceneSequence* EmbeddedSequence = Cast<UMovieSceneCinematicShotSection>(MasterShotTrack->GetAllSections()[IndexOfLastSection])->GetSequence();
    ULevelSequence* RemovedLevelSequence  = Cast<ULevelSequence>(EmbeddedSequence);

    // 3단계 마스터 시퀀스의 범위를 재설정
    ShrinkPlaybacksRange(RemovedLevelSequence);
    MasterShotTrack->RemoveSectionAt(IndexOfLastSection);

    // 4단계 재생할 범위를 재설정 
    UpdatePlaybackPosition(OutPlaybackParams, OutPlayToParams);
}
#pragma endregion

#pragma region IDialoguePositionController_Implementation_Private
UDialogueGraphNode* UDialoguePlayerInstance::GetPrevDialoguePosition()
{
    return PrevDialogueGraphNode;
}

UDialogueGraphNode* UDialoguePlayerInstance::GetCurrentDialoguePosition()
{
    return CurrentDialogueGraphNode;
}

#pragma endregion

#pragma region IDialoguePositionController_Implementation_Private

void UDialoguePlayerInstance::SetPrevDialoguePosition(UDialogueGraphNode* InDialogueGraphNode)
{
    PrevDialogueGraphNode = InDialogueGraphNode;
}

void UDialoguePlayerInstance::SetCurrentDialoguePosition(UDialogueGraphNode* InDialogueGraphNode)
{
    PrevDialogueGraphNode    = CurrentDialogueGraphNode;
    CurrentDialogueGraphNode = InDialogueGraphNode;
}
#pragma endregion
