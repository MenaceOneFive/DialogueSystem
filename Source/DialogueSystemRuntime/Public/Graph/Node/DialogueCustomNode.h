#pragma once
#include "DialogueLineNode.h"
#include "Graph/Node/DialogueConditionalNode.h"
#include "DialogueCustomNode.generated.h"

class UDialogueSystemEvent;

// 이벤트를 실행하는 노드 
UCLASS(BlueprintType)
class DIALOGUESYSTEMRUNTIME_API UDialogueCustomNode : public UDialogueConditionalNode
{
    GENERATED_BODY()

public:
    virtual void Accept(TScriptInterface<IRuntimeDialogueGraphVisitor> Visitor) const override;

    TSubclassOf<UDialogueSystemEvent> GetEventClass() const;

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

    virtual FText GetDefaultSelectionName() const override;;

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
#endif

    FSequencePlaySetting GetSequencePlaySetting() const;

#if WITH_EDITORONLY_DATA
    void SetEventClass(const TSubclassOf<UDialogueSystemEvent>& InEventClass);
#endif

protected:
    TSubclassOf<UDialogueSystemEvent> EventClass;

    UPROPERTY(VisibleAnywhere)
    TArray<FGuid> PrevNodeIDs; // 이전 노드의 ID들

    UPROPERTY(VisibleAnywhere)
    FGuid NextNodeID; // 다음 노드의 ID

    UPROPERTY(VisibleAnywhere)
    FSequencePlaySetting SequencePlaySetting;
};
