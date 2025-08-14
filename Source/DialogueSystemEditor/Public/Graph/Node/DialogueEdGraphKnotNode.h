// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueEdGraphNode.h"
#include "DialogueEdGraphKnotNode.generated.h"

/// <summary>
/// 연결선을 더블 클릭하면 생기는 노드 
/// 별 의미 없는 노드 (그래프 정리에 사용하는 노드)
/// </summary>
UCLASS()
class DIALOGUESYSTEMEDITOR_API UDialogueEdGraphKnotNode : public UDialogueEdGraphNode
{
    GENERATED_BODY()

public:
    virtual bool ShouldDrawNodeAsControlPointOnly(int32& OutInputPinIndex, int32& OutOutputPinIndex) const override;
    virtual void AllocateDefaultPins() override;
    TArray<TObjectPtr<const UDialogueEdGraphNode>> GetPrevNodes() const;

    virtual bool CanSplitPin(const UEdGraphPin* Pin) const override
    {
        return false;
    };


    /// <summary>
    /// 다음 노드를 반환 
    /// </summary>
    /// <returns>다음 노드</returns>
    TObjectPtr<const UDialogueEdGraphNode> GetNextNode() const;
    UEdGraphPin* GetInputPin();
    UEdGraphPin* GetOutputPin();
};
