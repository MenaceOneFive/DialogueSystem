// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "DialogueNode.h"
#include "UObject/Object.h"
#include "DialogueGraph.generated.h"

class UDialogueGraphNode;
class UDialoguePlayerInstance;
class IDialoguePlayer;
class ISectionStack;
class UDialogueStartNode;
class UDialogueSceneNode;
class UDialogueSelectionNode;

/**
 * 
 */
UCLASS(BlueprintType)
class DIALOGUESYSTEMRUNTIME_API UDialogueGraph : public UObject
{
    GENERATED_BODY()

public:
    void Initialize();

    virtual void BeginDestroy() override
    {
        UE_LOG(LogMemory, Display, TEXT("UDialogueGraph"))
        Super::BeginDestroy();
    };

#if WITH_EDITORONLY_DATA //런타임에는 이 그래프는 수정이 불가능해야 함
    UFUNCTION(BlueprintCallable)
    bool AddNode(UDialogueGraphNode* GraphNode);

    UFUNCTION(BlueprintCallable)
    UDialogueGraphNode* GetNode(const FGuid NodeID);

#endif

    UFUNCTION(BlueprintCallable)
    bool ContainsNode(const UDialogueGraphNode* GraphNode) const;

    const UDialogueGraphNode* GetNode(const FGuid NodeID) const;

    UFUNCTION(BlueprintCallable)
    UDialogueStartNode* GetStartNode() const;

private:
    UPROPERTY()
    bool bIsInitialized = false;

    UPROPERTY()
    UDialogueStartNode* StartNode;

    UPROPERTY()
    TMap<FGuid, UDialogueGraphNode*> Nodes;
};
