// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "DialoguePlayer.h"
#include "DialoguePositionController.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlaybackSettings.h"
#include "SectionStack.h"
#include "DataStructure/Graph/DialogueGraph.h"
#include "DataStructure/Graph/DialogueGraphVisitor.h"
#include "DialoguePlayerInstance.generated.h"

class UMovieScene;
class UMovieSceneSequence;
class UMovieSceneCinematicShotTrack;
class UMovieSceneCinematicShotSection;
class UDialogueGraphNode;
class ULevelSequence;
class ALevelSequenceActor;
class ULevelSequencePlayer;

/// <summary>
/// 대화 시스템을 재생하고 관리하는 핵심 클래스입니다.
/// 
/// 이 클래스는 대화 그래프(DialogueGraph)를 기반으로 시퀀스 기반 대화 시스템을 구현합니다.
/// 레벨 시퀀스를 활용하여 대화의 각 노드에 해당하는 시네마틱을 원활하게 재생하며,
/// 대화 그래프 노드 간의 전환과 진행 상태를 관리합니다.
/// 
/// 주요 기능:
/// - 마스터 레벨 시퀀스 관리: 전체 대화 흐름을 제어하는 메인 시퀀스를 초기화하고 관리합니다.
/// - 대화 노드 재생: 그래프의 각 노드에 해당하는 레벨 시퀀스를 재생합니다.
/// - 시퀀스 스택 관리: 대화 시퀀스를 스택 형태로 관리하여 복잡한 대화 흐름을 구현합니다.
/// - 대화 위치 추적: 현재 및 이전 대화 위치(노드)를 추적하여 대화 진행 상태를 관리합니다.
/// 
/// 구현된 인터페이스:
/// - IDialoguePlayer: 대화 재생 제어 (Play, PlayTo, Stop)
/// - ISectionStack: 시퀀스 섹션 스택 관리 (PushShotSection, PopShotSection)
/// - IDialoguePositionController: 대화 그래프 내 위치 제어 (GetCurrentDialoguePosition, SetCurrentDialoguePosition 등)
/// 
/// 대화 시스템 사용 시 중요 사항:
/// - 설정의 bPauseAtEnd 값은 반드시 True여야 합니다. False일 경우 OnPause 이벤트가 발생하지 않아 그래프 순회가 불가능합니다.
/// - 재생 중인 상태에서 Release를 호출하면 문제가 발생할 수 있습니다.
/// - 시퀀스의 프레임 관리는 틱레이트 기준으로 동작합니다.
/// 
/// 관련 클래스:
/// - UDialogueGraph
/// - UDialogueGraphNode
/// - IDialoguePlayer
/// - ISectionStack
/// - IDialoguePositionController
/// </summary>
UCLASS()
class UDialoguePlayerInstance final : public UObject,
                                      public ISectionStack,
                                      public IDialoguePlayer,
                                      public IDialoguePositionController
{
    GENERATED_BODY()
public:
    /// <summary>
    /// 대화를 재생할 준비를 합니다.
    /// </summary>
    /// <param name="MasterLevelSequence">현재 레벨에 대한 레벨 시퀀스</param>
    /// <param name="InDialogueGraph">재생할 대화</param>
    /// <param name="InSettings">대화 설정</param>
    void Initialize(ULevelSequence* MasterLevelSequence,
                    const UDialogueGraph* InDialogueGraph,
                    const FMovieSceneSequencePlaybackSettings& InSettings
                            = GetDefaultMovieSceneSequencePlaybackSettings());
    virtual void BeginDestroy() override;;

    static FMovieSceneSequencePlaybackSettings GetDefaultMovieSceneSequencePlaybackSettings();

private:
    void InitializeMasterScene(UMovieScene* MasterScene);

    /// <summary>
    /// 대화에 사용된 자원을 정리합니다.
    /// </summary>
    UFUNCTION()
    void Release();

    /// <summary>
    /// 다음 노드를 처리합니다.
    /// </summary>
    UFUNCTION()
    void HandleNextNode();

    /// <summary>
    /// 섹션을 생성
    /// </summary>
    /// <param name="SequenceToPlay">섹션에 담을 레벨 시퀀스</param>
    /// <returns>새 섹션</returns>
    UMovieSceneCinematicShotSection* CreateShotSectionWith(ULevelSequence* SequenceToPlay) const;

    /// <summary>
    /// 플레이 가능한 영역의 끝으로 섹션을 이동한다.
    /// </summary>
    /// <param name="NewShotSection">새로 만든 섹션</param>
    void MoveSectionToEnd(UMovieSceneCinematicShotSection* NewShotSection) const;

    /// <summary>
    /// 재생할 레벨 시퀀스를 이용해서 마스터씬 의 길이를 연장.
    /// </summary>
    /// <param name="SequenceToPlay">레벨 시퀀스</param>
    void ExpandPlaybacksRange(ULevelSequence* SequenceToPlay) const;

    /// <summary>
    /// 재생이 끝난 레벨 시퀀스를 이용해서 마스터신의 길이를 축소.
    /// </summary>
    /// <param name="RemovedSequence">레벨 시퀀스</param>
    void ShrinkPlaybacksRange(ULevelSequence* RemovedSequence) const;

    void UpdatePlaybackPosition(FMovieSceneSequencePlaybackParams& OutPlaybackParams,
                                FMovieSceneSequencePlayToParams& OutPlayToParams) const;

private:
    UPROPERTY()
    const UDialogueGraph* DialogueGraph; // 현재 대화의 흐름을 나타내는 그래프

    UPROPERTY()
    ALevelSequenceActor* LevelSequenceActor; // 현재 재생중인 대화의 시퀀스 플레이어

    UPROPERTY()
    ULevelSequencePlayer* LevelSequencePlayer; // 현재 재생중인 대화의 시퀀스 플레이어

    UPROPERTY()
    UMovieSceneCinematicShotTrack* MasterShotTrack; // 이 자원은 ISectionTrack을 사용하여 스택처럼 관리됩니다.

    UPROPERTY()
    TScriptInterface<IDialogueGraphVisitor> GraphVisitor; // 노드의 동작을 수행한다.

    FMovieSceneSequencePlaybackSettings Settings = GetDefaultMovieSceneSequencePlaybackSettings();

public:
#pragma region IDialoguePlayer_Implementation_Public

public:
    /// <summary>
    /// 대화를 재생합니다.
    /// </summary>
    virtual void Play() const override;

    /// <summary>
    /// 특정 위치까지 재생을 진행 
    /// </summary>
    /// <param name="PlaybackParams">재생과 관련된 설정 어디까지 재생할까를 기입</param>
    /// <param name="PlayToParams">TODO: 모름 실제 어떤 기능인지 확인해야 함</param>
    virtual void PlayTo(const FMovieSceneSequencePlaybackParams& PlaybackParams,
                        const FMovieSceneSequencePlayToParams& PlayToParams) const override;
    /// <summary>
    /// 재생을 영구적으로 멈춤
    /// </summary>
    virtual void Stop() const override;
#pragma endregion

#pragma region ISectionStack_Implementation_Private

private:
    virtual void PushShotSection(ULevelSequence* SequenceToPlay,
                                 FMovieSceneSequencePlaybackParams& OutPlaybackParams,
                                 FMovieSceneSequencePlayToParams& OutPlayToParams) const override;

    virtual void PopShotSection(FMovieSceneSequencePlaybackParams& OutPlaybackParams,
                                FMovieSceneSequencePlayToParams& OutPlayToParams) const override;
#pragma endregion

#pragma region IDialoguePositionController_Implementation_Private

private:
    virtual UDialogueGraphNode* GetPrevDialoguePosition() override;
    virtual UDialogueGraphNode* GetCurrentDialoguePosition() override;
    virtual void SetPrevDialoguePosition(UDialogueGraphNode* InDialogueGraphNode) override;
    virtual void SetCurrentDialoguePosition(UDialogueGraphNode* InDialogueGraphNode) override;

    UPROPERTY()
    UDialogueGraphNode* PrevDialogueGraphNode = nullptr;

    UPROPERTY()
    UDialogueGraphNode* CurrentDialogueGraphNode = nullptr;

    UPROPERTY()
    ULevelSequence* DuplicatedLevelSequence;
#pragma endregion
};
