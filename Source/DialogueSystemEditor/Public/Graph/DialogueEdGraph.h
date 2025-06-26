// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "Graph/DialogueGraphSchema.h"

#include "DialogueEdGraph.generated.h"


/// <summary>
/// 대화 그래프
/// </summary>
UCLASS()
class DIALOGUESYSTEMEDITOR_API UDialogueEdGraph : public UEdGraph
{
    GENERATED_BODY()

    explicit UDialogueEdGraph(const FObjectInitializer& Initializer): UEdGraph(Initializer)
    {
        Schema         = UDialogueGraphSchema::StaticClass();
        bEditable      = true;
        bAllowDeletion = true;
        bAllowRenaming = true;
    }

public:
    TArray<TObjectPtr<UDialogueEdGraphNode>> GetNodes() const;
};

UCLASS()
class DIALOGUESYSTEMEDITOR_API UDialogueEdGraphAssetData : public UAssetUserData
{
    GENERATED_BODY()

public:
    UPROPERTY()
    UDialogueEdGraph* DialogueEdGraph;
};
