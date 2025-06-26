#pragma once

#include "CoreMinimal.h"
#include "Graph/Node/DialogueEdGraphConditionalNode.h"
#include "DialogueEdGraphSelectNode.generated.h"

class FAbstractDialogueEdGraphVisitor;

UCLASS()
class DIALOGUESYSTEMEDITOR_API UDialogueEdGraphSelectNode : public UDialogueEdGraphConditionalNode
{
    GENERATED_BODY()

public: // UEdGraphNode
    virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    virtual void AllocateDefaultPins() override;
    virtual FText GetTooltipText() const override;
    void AddOutputPin();

public: // Visitor 패턴
    virtual void Accept(FAbstractDialogueEdGraphVisitor* Visitor) override;

    /// <summary>
    /// 현재 노드의 다음 노드들(선택지들)을 반환
    /// </summary>
    /// <returns>다음 노드(선택지들)</returns>
    TArray<const UDialogueEdGraphNode*> GetNextNodes() const;

    /// <summary>
    /// 이전 노드의 목록을 반환 
    /// </summary>
    /// <returns>이전 노드의 목록</returns>
    TArray<const UDialogueEdGraphNode*> GetPrevNodes() const;

private:
    int32 PinCounter = 0;
};
