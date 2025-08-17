#pragma once

#include "CoreMinimal.h"
#include "Graph/Node/DialogueEdGraphNode.h"
#include "DialogueEdGraphEndNode.generated.h"

class UDialogueEndNode;
class FAbstractDialogueEdGraphVisitor;

UCLASS()
class DIALOGUESYSTEMEDITOR_API UDialogueEdGraphEndNode : public UDialogueEdGraphNode
{
    GENERATED_BODY()

public: // UEdGraphNode
    UDialogueEdGraphEndNode();
    virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    virtual void AllocateDefaultPins() override;

public: // Visitor패턴 관련
    virtual void Accept(FAbstractDialogueEdGraphVisitor* Visitor) override;

    void CopyTo(const TObjectPtr<UDialogueEndNode>& EndNode) const;

    /// <summary>
    /// 이전 노드의 목록을 반환 
    /// </summary>
    /// <returns>이전 노드의 목록</returns>
    TArray<TObjectPtr<const UDialogueEdGraphNode>> GetPrevNodes() const;
};
