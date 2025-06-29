#pragma once
#include "Graph/Node/DialogueGraphNode.h"
#include "DialogueStartNode.generated.h"

// 대화의 시작을 나타내는 노드
UCLASS(BlueprintType)
class DIALOGUESYSTEMRUNTIME_API UDialogueStartNode : public UDialogueGraphNode
{
    GENERATED_BODY()

public:
    virtual void Accept(TObjectPtr<IRuntimeDialogueGraphVisitor> Visitor) const override;

    /// <summary>
    /// 다음 노드의 ID를 반환합니다.
    /// </summary>
    /// <returns>다음 노드의 GUID</returns>
    FGuid GetNextNodeID() const;

    /// <summary>
    /// 다음 노드의 ID를 설정합니다.
    /// </summary>
    /// <param name="InNextNodeID">설정할 다음 노드의 ID</param>
    void SetNextNodeID(const FGuid& InNextNodeID);

protected:
    UPROPERTY(VisibleAnywhere)
    FGuid NextNodeID; // 다음 노드의 ID
};
