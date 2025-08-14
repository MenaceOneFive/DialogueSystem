#pragma once
#include "Graph/Node/DialogueNodeInterface.h"
#include "DialogueGraphNode.generated.h"

class UDialogueGraphDirector;
class UDialogueSelectionItem;
class ULevelSequence;
class IRuntimeDialogueGraphVisitor;

UCLASS(BlueprintType, Abstract)
class DIALOGUESYSTEMRUNTIME_API UDialogueGraphNode : public UObject,
                                                     public IWhenSelectThisNode
{
    GENERATED_BODY()

public:
    UDialogueGraphNode();

    virtual void BeginDestroy() override;

public:
    /// 파생 클래스에서 Visitor의 어느 기능을 통해서 처리할지 정함
    /// 만약 이 클래스의 파생 클래스를 작성한다면, 파생 클래스에 특화된 코드를 Visitor에 구현해야 한다.
    /// <param name="Visitor">이 노드의 데이터를 사용할 Visitor구현</param>
    virtual void Accept(TScriptInterface<IRuntimeDialogueGraphVisitor> Visitor) const
    {
    };

    /// <summary>
    /// 노드의 고유 ID를 반환합니다.
    /// </summary>
    /// <returns>노드의 GUID</returns>
    FGuid GetNodeID() const;

    /// <summary>
    /// 노드의 고유 ID를 설정합니다.
    /// </summary>
    /// <param name="InNodeGuid">설정할 노드의 GUID</param>
    void SetNodeID(const FGuid& InNodeGuid);

public:
    virtual FName GetWhenSelectThisNodeFunctionName() const override;
    virtual void SetWhenSelectThisNodeFunctionName(FName InWhenSelectThisNodeFunctionName = NAME_None) override;

protected:
    UPROPERTY(VisibleAnywhere)
    FGuid NodeID; // 이 노드의 고유 ID
    UPROPERTY(VisibleAnywhere)
    FName WhenSelectThisNodeFunctionName = NAME_None;
};

using DialogueLineID = uint64; // TODO: 별도의 정의 파일로 이동
