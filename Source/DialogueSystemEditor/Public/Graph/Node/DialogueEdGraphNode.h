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

public: // UEdGraphNode
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
};
