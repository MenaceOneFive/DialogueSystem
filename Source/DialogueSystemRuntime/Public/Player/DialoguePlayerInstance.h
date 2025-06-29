// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "LevelSequenceActor.h"
#include "MovieSceneSequencePlaybackSettings.h"
#include "Graph/DialogueGraphVisitor.h"
#include "DialoguePlayerInstance.generated.h"

class UDialogueGraph;
class ULevelSequencePlayerHolder;
class UDialogueGraphDirector;
class UDialogueUIComponent;
/// <summary>
/// ULevelSequencePlayerActor 풀을 관리하는 클래스
/// UAlternativeDialoguePlayerSubsystem에 귀속됨
/// </summary>
UCLASS()
class UDialoguePlayerInstancePool final : public UObject
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
class ULevelSequencePlayerHolder : public UObject
{
    GENERATED_BODY()

public:
    void Initialize(UDialoguePlayerInstancePool* PlayerPool);

    virtual void BeginDestroy() override;

    void SetLevelSequencePlayerActor(ALevelSequenceActor* InPlayerActor);

    ULevelSequencePlayer* GetLevelSequencePlayer() const;

    void SetLevelSequence(ULevelSequence* LevelSequenceToPlay) const;

private:
    UPROPERTY()
    UDialoguePlayerInstancePool* ParentPool;

    UPROPERTY()
    ALevelSequenceActor* SequencePlayerActor;
};


UCLASS(BlueprintType)
class UDialogueRuntimePlayer : public UObject,
                               public IRuntimeDialogueGraphVisitor
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void PlayFromBeginning();

    UFUNCTION(BlueprintCallable)
    void Pause();

    UFUNCTION(BlueprintCallable)
    void Stop();

    UFUNCTION(BlueprintCallable)
    void SetDialogueGraph(const UDialogueGraph* InDialogueGraph);

public: // IDialogueGraphVisitor 구현
    virtual void InitializeVisitor() override;
    virtual void VisitStartNode(const TObjectPtr<const UDialogueStartNode>& StartNode) override;
    virtual void VisitSelectionNode(const TObjectPtr<const UDialogueSelectionNode>& SelectionNode) override;
    virtual void VisitSceneNode(const TObjectPtr<const UDialogueSceneNode>& SceneNode) override;
    virtual void VisitEndNode(const TObjectPtr<const UDialogueEndNode>& EndNode) override;
    virtual void VisitEventNode(const TObjectPtr<const UDialogueEventNode>& EventNode) override;
    void WhenVisitThisNode(const TObjectPtr<const UDialogueGraphNode>& InPrevNode,
                           const TObjectPtr<const UDialogueGraphNode>& InCurrentNode) const;

    void SkipCurrentNode() const;

private:
    UFUNCTION()
    void VisitNextNode();

    void InitializePlayers();

private:
    void HideSubtitle() const;

    void FocusOnRootWidget() const;

    void UnfocusFromWidget() const;

    void ShowCursor() const;

    void HideCursor() const;

    // 현재 재생에 사용하는 그래프
    UPROPERTY()
    TObjectPtr<const UDialogueGraph> DialogueGraph;

    // 현재 SceneNode의 재생을 담당하는 플레이어를 담은 홀더
    UPROPERTY()
    TObjectPtr<ULevelSequencePlayerHolder> CurrentHolder;

    // 조금 더 빠르게 Holder를 탐색하기 위해 만든 Map
    UPROPERTY()
    TMap<TObjectPtr<const UDialogueSceneNode>, TObjectPtr<ULevelSequencePlayerHolder>> SceneNodeToPlayerHolder;

    // 다음 노드에 대한 참조
    UPROPERTY()
    TObjectPtr<const UDialogueGraphNode> NextNode;

    // 이전 노드에 대한 참조
    UPROPERTY()
    TObjectPtr<const UDialogueGraphNode> PrevNode;

    UPROPERTY()
    TObjectPtr<UDialogueGraphDirector> DialogueGraphDirector;
};
