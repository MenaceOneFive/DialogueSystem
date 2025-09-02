// Fill out your copyright notice in the Description page of Project Settings.

#include "Subsystem/DialoguePlayerSubsystem.h"

#include "AbilitySystemInterface.h"
#include "Player/DialoguePlayerInstance.h"
#include "Player/LevelSequencePlayerPool.h"

void UDialoguePlayerRuntimeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    PlayerResourcePool = NewObject<ULevelSequencePlayerInstancePool>(this);
    PlayerResourcePool->InitializeSequencePlayerPool(5);
}

UDialogueRuntimePlayer* UDialoguePlayerRuntimeSubsystem::CreateDialoguePlayerFromDialogueGraph(UObject*              ContextObject,
                                                                                               const UDialogueGraph* DialogueGraph)
{
    const auto RuntimePlayer = NewObject<UDialogueRuntimePlayer>(ContextObject);
    RuntimePlayer->SetDialogueGraph(DialogueGraph);
    return RuntimePlayer;
}

ULevelSequencePlayerHolder* UDialoguePlayerRuntimeSubsystem::RequestNewPlayerInstance() const
{
    return PlayerResourcePool->RequestNewPlayerInstance();
}

void UDialogueUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

void UDialogueUISubsystem::Deinitialize()
{
    Super::Deinitialize();
}
