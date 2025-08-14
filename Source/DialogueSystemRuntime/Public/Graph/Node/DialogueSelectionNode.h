#pragma once
#include "Graph/Node/DialogueConditionalNode.h"
#include "DialogueSelectionNode.generated.h"

class UDialogueSelectionItem;

// 선택지를 표시하는 노드 
UCLASS(BlueprintType)
class DIALOGUESYSTEMRUNTIME_API UDialogueSelectionNode : public UDialogueConditionalNode
{
    GENERATED_BODY()

public:
    virtual void Accept(TScriptInterface<IRuntimeDialogueGraphVisitor> Visitor) const override;

    /// <summary>
    /// 현재 노드의 선택지 항목들을 반환합니다.
    /// </summary>
    /// <returns>선택지 항목 배열</returns>
    TArray<UDialogueSelectionItem*> GetSelectionItems() const;

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

    /// <summary>
    /// 선택지에 따른 다음 노드들의 ID 배열을 반환합니다.
    /// </summary>
    /// <returns>다음 노드들의 GUID 배열</returns>
    TArray<FGuid> GetNextNodeIDs() const;

    /// <summary>
    /// 선택지에 따른 다음 노드들의 ID 배열을 설정합니다.
    /// </summary>
    /// <param name="InNextNodeIDs">설정할 다음 노드들의 ID 배열</param>
    void SetNextNodeIDs(const TArray<FGuid>& InNextNodeIDs);

    /// <summary>
    /// 선택지 항목들을 설정합니다.
    /// </summary>
    /// <param name="Items">설정할 선택지 항목 배열</param>
    void SetSelectionItems(const TArray<UDialogueSelectionItem*>& Items);

protected:
    UPROPERTY(VisibleAnywhere)
    TArray<TObjectPtr<UDialogueSelectionItem>> SelectionItems;

    UPROPERTY(VisibleAnywhere)
    TArray<FGuid> PrevNodeIDs; // 이전 노드의 ID들

    UPROPERTY(VisibleAnywhere)
    TArray<FGuid> NextNodeIDs; // 다음 노드들의 ID 배열
};
