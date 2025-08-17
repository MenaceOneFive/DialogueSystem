#pragma once
#include "Graph/Node/DialogueGraphNode.h"
#include "DialogueEndNode.generated.h"

// 종료 노드
UCLASS(BlueprintType)
class DIALOGUESYSTEMRUNTIME_API UDialogueEndNode : public UDialogueGraphNode
{
    GENERATED_BODY()

public:
    virtual void Accept(TScriptInterface<IRuntimeDialogueGraphVisitor> Visitor) const override;

    /// <summary>
    /// 이전 노드들의 ID 배열을 반환합니다.
    /// </summary>
    /// <returns>이전 노드들의 GUID 배열</returns>
    TArray<FGuid> GetPrevNodeIDs() const;

    /// <summary>
    /// 이전 노드들의 ID 배열을 설정합니다.
    /// </summary>
    /// <param name="InPrevNodeIDs">설정할 이전 노드들의 ID 배열</param>
    void SetPrevNodeIDs(const TArray<FGuid>& InPrevNodeIDs);

protected:
    UPROPERTY(VisibleAnywhere)
    TArray<FGuid> PrevNodeIDs; // 이전 노드의 ID들
};
