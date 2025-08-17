#pragma once
#include "SequencePlayCapability.h"
#include "SkippableNode.h"
#include "Graph/Node/DialogueConditionalNode.h"
#include "DialogueLineNode.generated.h"

class ULevelSequence;

// LevelSequence를 재생하는 노드
UCLASS(BlueprintType)
class DIALOGUESYSTEMRUNTIME_API UDialogueLineNode : public UDialogueConditionalNode,
                                                    public ISequencePlayCapability,
                                                    public ISkippableNode
{
    GENERATED_BODY()

public:
    virtual void Accept(TScriptInterface<IRuntimeDialogueGraphVisitor> Visitor) const override;

    virtual FText GetDefaultSelectionName() const override;;

    /// <summary>
    /// 재생할 레벨 시퀀스를 반환합니다.
    /// </summary>
    /// <returns>재생할 레벨 시퀀스 객체</returns>
    virtual TSoftObjectPtr<ULevelSequence> GetLevelSequenceToPlay() const override;

    /// <summary>
    /// 이전 노드들의 ID 배열을 반환합니다.
    /// </summary>
    /// <returns>이전 노드들의 GUID 배열</returns>
    TArray<FGuid> GetPrevNodeIDs() const;

    /// <summary>
    /// 다음 노드의 ID를 반환합니다.
    /// </summary>
    /// <returns>다음 노드의 GUID</returns>
    FGuid GetNextNodeID() const;

    virtual FSequencePlaySetting GetSequencePlaySetting() const override;

#if WITH_EDITORONLY_DATA
    /// <summary>
    /// 이전 노드들의 ID 배열을 설정합니다.
    /// </summary>
    /// <param name="InPrevNodeIDs">설정할 이전 노드들의 ID 배열</param>
    void SetPrevNodeIDs(const TArray<FGuid>& InPrevNodeIDs);

    /// <summary>
    /// 다음 노드의 ID를 설정합니다.
    /// </summary>
    /// <param name="InNextNodeID">설정할 다음 노드의 ID</param>
    void SetNextNodeID(const FGuid& InNextNodeID);

    /// <summary>
    /// 재생할 레벨 시퀀스를 설정합니다.
    /// </summary>
    /// <param name="InLevelSequenceToPlay">설정할 레벨 시퀀스 객체</param>
    virtual void SetLevelSequenceToPlay(const TSoftObjectPtr<ULevelSequence>& InLevelSequenceToPlay) override;

    virtual void SetSequencePlaySetting(const FSequencePlaySetting& InSequencePlaySetting) override;
#endif

public: // ISkippableNode
    virtual bool CanSkipThisNode() const override;

protected:
    UPROPERTY(VisibleAnywhere)
    TSoftObjectPtr<ULevelSequence> LevelSequenceToPlay = nullptr;

    UPROPERTY(VisibleAnywhere)
    TArray<FGuid> PrevNodeIDs; // 이전 노드의 ID들

    UPROPERTY(VisibleAnywhere)
    FGuid NextNodeID; // 다음 노드의 ID

    UPROPERTY(VisibleAnywhere)
    FSequencePlaySetting SequencePlaySetting;
};
