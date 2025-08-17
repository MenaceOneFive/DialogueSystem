// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "DialoguePlaySetting.h"
#include "DialogueSystemRuntimeModule.h"
#include "LevelSequenceActor.h"
#include "Graph/DialogueGraphVisitor.h"
#include "Widget/DialoguePlayerUIRoot.h"
#include "DialoguePlayerInstance.generated.h"

class ULevelSequence;
class UDialogueGraph;
class ULevelSequencePlayerHolder;
class UDialogueGraphDirector;
class UDialogueUIComponent;


UCLASS(BlueprintType)
class DIALOGUESYSTEMRUNTIME_API UDialogueGraphVisitNextDelegateExecutor : public UObject
{
    GENERATED_BODY()

public:
    void Initialize(const FSimpleDelegate& InVisitNextDelegate);;

    UFUNCTION(BlueprintCallable)
    void VisitNextNode();

private:
    FSimpleDelegate OnVisitNextDelegate;
};

UINTERFACE()
class UDialogueCustomNodeSequencePlayer : public UInterface
{
    GENERATED_BODY()
};


class IDialogueCustomNodeSequencePlayer
{
    GENERATED_BODY()

public:
    virtual TObjectPtr<ULevelSequencePlayerHolder> GetPlayerHolder(const TObjectPtr<const UDialogueCustomNode>& GraphNode) = 0;
    virtual void PlaySequence(const TObjectPtr<const UDialogueCustomNode>& InCustomNode, const TObjectPtr<ULevelSequencePlayerHolder>& Holder) = 0;
};


UINTERFACE()
class UDialogueUIManager : public UInterface
{
    GENERATED_BODY()
};

class IDialogueUIManager
{
    GENERATED_BODY()

public:
    virtual void HideSubtitle() const = 0;

    virtual void FocusOnRootWidget() const = 0;

    virtual void UnfocusFromWidget() const = 0;

    virtual void ShowCursor() const = 0;

    virtual void HideCursor() const = 0;

    /// <summary>
    /// 키 입력 바인딩을 추가합니다.
    /// <param name="Key">바인드할 키</param>
    /// <param name="InEvent">호출될 동적 델리게이트(파라미터 없음)</param>
    /// </summary>
    virtual void BindKeyDownEvent(FKey Key, FDialoguePlayerKeyboardEvent InEvent) = 0;

    /// <summary>
    /// 키 업 입력 바인딩을 추가합니다.
    /// <param name="Key">바인드할 키</param>
    /// <param name="InEvent">호출될 동적 델리게이트(파라미터 없음)</param>
    /// </summary>
    virtual void BindKeyUpEvent(FKey Key, FDialoguePlayerKeyboardEvent InEvent) = 0;

    /// <summary>
    /// 마우스 버튼 다운 입력 바인딩을 추가합니다.
    /// <param name="MouseButton">바인드할 마우스 버튼</param>
    /// <param name="InEvent">호출될 동적 델리게이트(파라미터: InMouseEvent)</param>
    /// </summary>
    virtual void BindMouseButtonDownEvent(FKey MouseButton, FDialoguePlayerUIMouseEvent InEvent) = 0;

    /// <summary>
    /// 마우스 버튼 업 입력 바인딩을 추가합니다.
    /// <param name="MouseButton">바인드할 마우스 버튼</param>
    /// <param name="InEvent">호출될 동적 델리게이트(파라미터: InMouseEvent)</param>
    /// </summary>
    virtual void BindMouseButtonUpEvent(FKey MouseButton, FDialoguePlayerUIMouseEvent InEvent) = 0;

    /// <summary>
    /// 마우스 버튼 더블 클릭 입력 바인딩을 추가합니다.
    /// <param name="MouseButton">바인드할 마우스 버튼</param>
    /// <param name="InEvent">호출될 동적 델리게이트(파라미터: InMouseEvent)</param>
    /// </summary>
    virtual void BindMouseButtonDoubleClickEvent(FKey MouseButton, FDialoguePlayerUIMouseEvent InEvent) = 0;

    /// <summary>
    /// 키 다운 바인딩을 전부 제거합니다.
    /// <param name="Key">해제할 키</param>
    /// </summary>
    virtual void UnbindKeyDownEvent(FKey Key) = 0;

    /// <summary>
    /// 키 업 바인딩을 전부 제거합니다.
    /// <param name="Key">해제할 키</param>
    /// </summary>
    virtual void UnbindKeyUpEvent(FKey Key) = 0;

    /// <summary>
    /// 해당 마우스 버튼의 다운 바인딩을 전부 제거합니다.
    /// <param name="MouseButton">해제할 마우스 버튼</param>
    /// </summary>
    virtual void UnbindMouseButtonDownEvent(FKey MouseButton) = 0;

    /// <summary>
    /// 해당 마우스 버튼의 업 바인딩을 전부 제거합니다.
    /// <param name="MouseButton">해제할 마우스 버튼</param>
    /// </summary>
    virtual void UnbindMouseButtonUpEvent(FKey MouseButton) = 0;

    /// <summary>
    /// 해당 마우스 버튼의 더블 클릭 바인딩을 전부 제거합니다.
    /// <param name="MouseButton">해제할 마우스 버튼</param>
    /// </summary>
    virtual void UnbindMouseButtonDoubleClickEvent(FKey MouseButton) = 0;

    /// <summary>
    /// 모든 입력 바인딩을 초기화합니다.
    /// </summary>
    virtual void ClearAllBindings() = 0;
};

UCLASS(BlueprintType)
class DIALOGUESYSTEMRUNTIME_API UDialogueRuntimePlayer : public UObject,
                                                         public IDialogueCustomNodeSequencePlayer,
                                                         public IDialogueUIManager,
                                                         public IRuntimeDialogueGraphVisitor
{
    GENERATED_BODY()

public: // 외부에 노출된 재생 관련 메서드
    UFUNCTION(BlueprintCallable)
    void PlayFromBeginning();

    UFUNCTION(BlueprintCallable)
    void Pause();

    UFUNCTION(BlueprintCallable)
    void Stop();

    UFUNCTION(BlueprintCallable)
    void SetDialogueGraph(const UDialogueGraph* InDialogueGraph);

    void SkipCurrentNode() const;

public:
    virtual TObjectPtr<ULevelSequencePlayerHolder> GetPlayerHolder(const TObjectPtr<const UDialogueCustomNode>& GraphNode) override;
    virtual void PlaySequence(const TObjectPtr<const UDialogueCustomNode>& InCustomNode, const TObjectPtr<ULevelSequencePlayerHolder>& Holder) override;

    // IDialogueGraphVisitor 구현
    virtual void InitializeVisitor() override;
    virtual void VisitStartNode(const TObjectPtr<const UDialogueStartNode>& StartNode) override;
    virtual void VisitSelectionNode(const TObjectPtr<const UDialogueSelectionNode>& SelectionNode) override;
    virtual void VisitSceneNode(const TObjectPtr<const UDialogueLineNode>& SceneNode) override;
    virtual void VisitEndNode(const TObjectPtr<const UDialogueEndNode>& EndNode) override;
    virtual void VisitCustomNode(const TObjectPtr<const UDialogueCustomNode>& CustomNode) override;

private: // DialogueGraphDirector
    void WhenVisitThisNode(const TObjectPtr<const UDialogueGraphNode>& InPrevNode,
                           const TObjectPtr<const UDialogueGraphNode>& InCurrentNode) const;
    bool CanSelectNode(const FGuid& NodeGuid) const;

private: // Player관련 Helper
    void InitializePlayers();

    void InitializeSequencePlayer(const ULevelSequencePlayerHolder* Holder, const UDialogueGraphNode* CurrentNode, FGuid NextNodeId);

    void VisitNextNode(const UDialogueGraphNode* CurrentNode, FGuid NextNodeId);

    void VisitNextNodeFromSelection(const UDialogueGraphNode* CurrentNode, FGuid NextNodeId);

    void VisitNextNodeFromScene(const ULevelSequencePlayerHolder* InCurrentHolder, const UDialogueGraphNode* InCurrentNode, FGuid InNextNodeId);

    FSimpleDelegate MakeVisitNextNodeDelegate(const ULevelSequencePlayerHolder* InCurrentHolder, const UDialogueGraphNode* InCurrentNode, FGuid InNextNodeId);

    FSimpleDelegate MakeVisitNextNodeFromSelectionDelegate(const UDialogueGraphNode* InCurrentNode, FGuid InNextNodeId);;

    void ApplySequencePlaybackSetting(ULevelSequencePlayer* Player, const FDialogueSetting& DialogueSetting, const FSequencePlaySetting& PlaySetting);

private: // UI 관련
    void ApplyDialogueUISetting(FDialogueSetting DialogueSetting) const;

    virtual void HideSubtitle() const override;

    virtual void FocusOnRootWidget() const override;

    virtual void UnfocusFromWidget() const override;

    virtual void ShowCursor() const override;

    virtual void HideCursor() const override;

    /// <summary>
    /// 키 입력 바인딩을 추가합니다.
    /// <param name="Key">바인드할 키</param>
    /// <param name="InEvent">호출될 동적 델리게이트(파라미터 없음)</param>
    /// </summary>
    virtual void BindKeyDownEvent(FKey Key, FDialoguePlayerKeyboardEvent InEvent) override;

    /// <summary>
    /// 키 업 입력 바인딩을 추가합니다.
    /// <param name="Key">바인드할 키</param>
    /// <param name="InEvent">호출될 동적 델리게이트(파라미터 없음)</param>
    /// </summary>
    virtual void BindKeyUpEvent(FKey Key, FDialoguePlayerKeyboardEvent InEvent) override;

    /// <summary>
    /// 마우스 버튼 다운 입력 바인딩을 추가합니다.
    /// <param name="MouseButton">바인드할 마우스 버튼</param>
    /// <param name="InEvent">호출될 동적 델리게이트(파라미터: InMouseEvent)</param>
    /// </summary>
    virtual void BindMouseButtonDownEvent(FKey MouseButton, FDialoguePlayerUIMouseEvent InEvent) override;

    /// <summary>
    /// 마우스 버튼 업 입력 바인딩을 추가합니다.
    /// <param name="MouseButton">바인드할 마우스 버튼</param>
    /// <param name="InEvent">호출될 동적 델리게이트(파라미터: InMouseEvent)</param>
    /// </summary>
    virtual void BindMouseButtonUpEvent(FKey MouseButton, FDialoguePlayerUIMouseEvent InEvent) override;

    /// <summary>
    /// 마우스 버튼 더블 클릭 입력 바인딩을 추가합니다.
    /// <param name="MouseButton">바인드할 마우스 버튼</param>
    /// <param name="InEvent">호출될 동적 델리게이트(파라미터: InMouseEvent)</param>
    /// </summary>
    virtual void BindMouseButtonDoubleClickEvent(FKey MouseButton, FDialoguePlayerUIMouseEvent InEvent) override;

    /// <summary>
    /// 키 다운 바인딩을 전부 제거합니다.
    /// <param name="Key">해제할 키</param>
    /// </summary>
    virtual void UnbindKeyDownEvent(FKey Key) override;

    /// <summary>
    /// 키 업 바인딩을 전부 제거합니다.
    /// <param name="Key">해제할 키</param>
    /// </summary>
    virtual void UnbindKeyUpEvent(FKey Key) override;

    /// <summary>
    /// 해당 마우스 버튼의 다운 바인딩을 전부 제거합니다.
    /// <param name="MouseButton">해제할 마우스 버튼</param>
    /// </summary>
    virtual void UnbindMouseButtonDownEvent(FKey MouseButton) override;

    /// <summary>
    /// 해당 마우스 버튼의 업 바인딩을 전부 제거합니다.
    /// <param name="MouseButton">해제할 마우스 버튼</param>
    /// </summary>
    virtual void UnbindMouseButtonUpEvent(FKey MouseButton) override;

    /// <summary>
    /// 해당 마우스 버튼의 더블 클릭 바인딩을 전부 제거합니다.
    /// <param name="MouseButton">해제할 마우스 버튼</param>
    /// </summary>
    virtual void UnbindMouseButtonDoubleClickEvent(FKey MouseButton) override;

    /// <summary>
    /// 모든 입력 바인딩을 초기화합니다.
    /// </summary>
    virtual void ClearAllBindings() override;

    FText GetSelectionName(const TObjectPtr<const UDialogueGraphNode>& Node);

private:
    // 현재 SceneNode의 재생을 담당하는 플레이어를 담은 홀더
    UPROPERTY()
    TObjectPtr<ULevelSequencePlayerHolder> CurrentHolder;

    // 다음 노드에 대한 참조
    UPROPERTY()
    TObjectPtr<const UDialogueGraphNode> NextNode;

    // 이전 노드에 대한 참조
    UPROPERTY()
    TObjectPtr<const UDialogueGraphNode> PrevNode;

    // 현재 재생에 사용하는 그래프
    UPROPERTY()
    TObjectPtr<const UDialogueGraph> DialogueGraph;

private:
    const FGuid InvalidGuid = FGuid();

    // 조금 더 빠르게 Holder를 탐색하기 위해 만든 Map
    UPROPERTY()
    TMap<TObjectPtr<const UDialogueGraphNode>, TObjectPtr<ULevelSequencePlayerHolder>> SceneNodeToPlayerHolder;

    UPROPERTY()
    TObjectPtr<UDialogueGraphDirector> DialogueGraphDirector;

    UPROPERTY()
    TObjectPtr<UDialogueUIComponent> UIComponent;

    // PlayerController에서 UDialogueUIComponent를 안전하게 얻어오는 헬퍼
    UDialogueUIComponent* ResolveUIComponent() const;

    FDialogueSystemRuntimeModule* Module;
};
