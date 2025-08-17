// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueSystemRuntime/RuntimeCustomNodeManager.h"
#include "Graph/Node/DialogueCustomNode.h"
#include "Graph/Node/DialogueSystemCustomNodeDefinition.h"
#include "DialogueSystemRuntimeModule.generated.h"

class UDialogueCustomNode;
class FDialogueSystemCustomNodeDefinition;

UCLASS()
class UDialogueQTENode : public UDialogueCustomNode
{
    GENERATED_BODY()

public:
    explicit UDialogueQTENode();
    virtual FText GetDefaultSelectionName() const override;

    UPROPERTY(EditAnywhere)
    FString Name;

    UPROPERTY(EditAnywhere)
    TSoftObjectPtr<ULevelSequence> LevelSequenceToPlay;
    FKey RequiredKey;
};

struct FDialogueQTENodeDefinition final : FDialogueSystemCustomNodeDefinition
{
    virtual TSubclassOf<UDialogueCustomNode> GetRuntimeCustomNodeType() const override;
    virtual void EvaluateCustomNode(const UDialogueCustomNode* CustomNode, UObject* Evaluator) const override;
};


class DIALOGUESYSTEMRUNTIME_API FDialogueSystemRuntimeModule final : public IModuleInterface
{
public:
    virtual void ShutdownModule() override;
    virtual void StartupModule() override;

    FRuntimeCustomNodeManager* GetCustomNodeManager() const;

private:
    TUniquePtr<FRuntimeCustomNodeManager> CustomNodeManager;
};
