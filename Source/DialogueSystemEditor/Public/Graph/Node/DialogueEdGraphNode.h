// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "EdGraph/EdGraphNode.h"
#include "Graph/Node/DialogueGraphNode.h"
#include "DialogueEdGraphNode.generated.h"

class FAbstractDialogueEdGraphVisitor;

/// <summary>
/// DialogueGraph에서 사용되는 Node들의 베이스 타입
/// </summary>
UCLASS(Abstract)
class DIALOGUESYSTEMEDITOR_API UDialogueEdGraphNode : public UEdGraphNode,
                                                      public IWhenSelectThisNode
{
    GENERATED_BODY()

#pragma region PinCategory

public:
    static const FName PC_Flow;
    static const FName PC_Exec;
    static const FName PC_Wildcard;
#pragma endregion

public:
    UDialogueEdGraphNode();

public:
    FDialogueSetting GetDialogueSetting() const;
    void SetDialogueSetting(const FDialogueSetting& InDialogueSetting);

    // UEdGraphNode
    bool IsAllowRecursiveConnection() const;
    virtual void PostInitProperties() override;
    virtual void PostPlacedNewNode() override;
    virtual void AutowireNewNode(UEdGraphPin* FromPin) override;

    void SetNodePosition(int32 InPosX,
                         int32 InPosY);

public: // Visitor패턴
    virtual void Accept(FAbstractDialogueEdGraphVisitor* Visitor) PURE_VIRTUAL(UDialogueEdGraphNode::Accept,);

public: // IWhenSelectThisNode
    virtual FName GetWhenSelectThisNodeFunctionName() const override;
    virtual void SetWhenSelectThisNodeFunctionName(FName InWhenSelectThisNodeFunctionName = NAME_None) override;

protected: // UEdGraphNode
    bool bAllowRecursiveConnection = false;

protected: // IWhenSelectThisNode
    UPROPERTY()
    FName WhenSelectThisNodeFunctionName = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDialogueSetting DialogueSetting;
};

template <typename DialogueEdNodeType>
TObjectPtr<const UDialogueEdGraphNode> GetNextNode(const DialogueEdNodeType* Node)
{
    static_assert(TIsDerivedFrom<DialogueEdNodeType, UDialogueEdGraphNode>::IsDerived, "DialogueEdNodeType는 UDialogueEdGraphNode여야 합니다.");

    // 출력 1개만 갖고 있다고 가정한다.
    auto* FindPinPtr = Algo::FindByPredicate(Node->Pins, [](const UEdGraphPin* Pin)
    {
        return Pin->Direction == EGPD_Output;
    });

    // 연결이 없는 경우 nullptr를 반환
    if (!FindPinPtr || !*FindPinPtr)
    {
        return nullptr;
    }

    // 출력에 연결되는 입력은 1개만 있다고 가정한다.
    if (const UEdGraphPin* PinToNext = *FindPinPtr; !PinToNext->LinkedTo.IsEmpty())
    {
        if (const UEdGraphPin* LinkedTo = PinToNext->LinkedTo[0])
        {
            return Cast<const UDialogueEdGraphNode>(LinkedTo->GetOwningNode());
        }
    }
    return nullptr;
}

template <typename DialogueEdNodeType>
TArray<TObjectPtr<const UDialogueEdGraphNode>> GetNextNodes(const DialogueEdNodeType* Node)
{
    static_assert(TIsDerivedFrom<DialogueEdNodeType, UDialogueEdGraphNode>::IsDerived, "DialogueEdNodeType는 UDialogueEdGraphNode여야 합니다.");

    // 출력핀만 필터링한다.
    TArray<UEdGraphPin*> OutputPins;
    Algo::CopyIf(Node->Pins, OutputPins, [](const UEdGraphPin* Pin)
    {
        return Pin->Direction == EGPD_Output;
    });

    // 핀에 해당하는 노드를 찾아서 배열에 추가한다.
    TArray<TObjectPtr<const UDialogueEdGraphNode>> OutputNodes;
    for (UEdGraphPin* OutputPin : OutputPins)
    {
        if (OutputPin->LinkedTo.IsEmpty())
        {
            continue;
        }
        if (const auto LinkedTo = OutputPin->LinkedTo[0])
        {
            if (auto OwningNode = Cast<const UDialogueEdGraphNode>(LinkedTo->GetOwningNode()))
            {
                OutputNodes.Emplace(OwningNode);
            }
        }
    }

    return OutputNodes;
}

template <typename DialogueEdNodeType>
TArray<TObjectPtr<const UDialogueEdGraphNode>> GetPrevNodes(const DialogueEdNodeType* Node)
{
    // 타입 체킹
    static_assert(TIsDerivedFrom<DialogueEdNodeType, UDialogueEdGraphNode>::IsDerived, "DialogueEdNodeType는 UDialogueEdGraphNode여야 합니다.");

    const auto InputPin = Algo::FindByPredicate(Node->Pins, [](const UEdGraphPin* Pin)
    {
        return Pin->Direction == EGPD_Input;
    });

    checkf(InputPin, TEXT("InputPin은 무조건 존재해야 합니다."))

    // 한 핀에 들어 올 수 있는 입력은 여러개일 수 있다.
    TArray<TObjectPtr<const UDialogueEdGraphNode>> Output;

    // 입력핀과 연결되어 있는 핀들의 소유주를 목록화 해서 반환
    for (const auto& LinkedPin : (*InputPin)->LinkedTo)
    {
        if (const auto PrevNode = Cast<UDialogueEdGraphNode>(LinkedPin->GetOwningNode()))
        {
            Output.Emplace(PrevNode);
        }
    }
    return Output;
}
