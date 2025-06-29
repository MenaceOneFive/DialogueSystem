#pragma once

#include "CoreMinimal.h"
#include "Graph/Node/DialogueEdGraphNode.h"
#include "DialogueEdGraphStartNode.generated.h"

class FAbstractDialogueEdGraphVisitor;

UCLASS()
class DIALOGUESYSTEMEDITOR_API UDialogueEdGraphStartNode : public UDialogueEdGraphNode
{
	GENERATED_BODY()

public: // UEdGraphNode
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void AllocateDefaultPins() override;

public: // Visitor 패턴
	virtual void Accept(FAbstractDialogueEdGraphVisitor* Visitor) override;

	/// <summary>
	/// 다음 노드를 반환 
	/// </summary>
	/// <returns>다음 노드</returns>
	TObjectPtr<const UDialogueEdGraphNode> GetNextNode() const;
};
