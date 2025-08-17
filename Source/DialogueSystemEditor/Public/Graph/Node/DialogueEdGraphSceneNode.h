#pragma once

#include "CoreMinimal.h"
#include "Graph/Node/DialogueEdGraphConditionalNode.h"
#include "DialogueEdGraphSceneNode.generated.h"

UCLASS(Abstract)
class DIALOGUESYSTEMEDITOR_API UDialogueEdGraphSceneNode : public UDialogueEdGraphConditionalNode
{
    GENERATED_BODY()

public: // UEdGraphNode

    virtual FText GetDefaultSelectionName() const override;
    virtual void AllocateDefaultPins() override;

    virtual void Accept(FAbstractDialogueEdGraphVisitor* Visitor) override;

    /// <summary>
    /// 이전 노드의 목록을 반환 
    /// </summary>
    /// <returns>이전 노드의 목록</returns>
    TArray<TObjectPtr<const UDialogueEdGraphNode>> GetPrevNodes() const;

    /// <summary>
    /// 다음 노드를 반환 
    /// </summary>
    /// <returns>다음 노드</returns>
    TObjectPtr<const UDialogueEdGraphNode> GetNextNode() const;
};
