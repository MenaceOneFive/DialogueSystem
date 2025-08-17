// Fill out your copyright notice in the Description page of Project Settings.
#include "DialogueSystemEvent.h"
#include "GameFramework/Character.h"
#include "Subsystem/DialoguePlayerSubsystem.h"

void UDialogueSystemEvent::StartEvent()
{
    ensureMsgf(!EventName.IsEmpty(), TEXT("이벤트의 이름을 가급적이면 작성해주세요."));

    // Player에서 적절히 초기화하지 못한 경우
    checkf(OnVisitNextNode.IsBound(), TEXT("이 노드가 끝난 뒤 무엇을 할지 정의되지 않았습니다"));

    UE_LOG(LogTemp, Log, TEXT("%s 이벤트를 시작합니다."), *EventName)
    Execute();
}

void UDialogueSystemEvent::Execute_Implementation()
{
}


void UDialogueSystemEvent::EndEvent() const
{
    UE_LOG(LogTemp, Log, TEXT("%s 이벤트를 종료합니다."), *EventName)

    // 다음 노드를 재생
    (void)OnVisitNextNode.ExecuteIfBound();
}

ACharacter* UDialogueSystemEvent::GetCharacter() const
{
    return GetTypedOuter<ACharacter>();
}

UWorld* UDialogueSystemEvent::GetCharacterWorld() const
{
    return GetCharacter()->GetWorld();
}

UDialoguePlayerRuntimeSubsystem* UDialogueSystemEvent::GetDialoguePlayerRuntimeSubsystem() const
{
    return GetCharacter()->GetGameInstance()->GetSubsystem<UDialoguePlayerRuntimeSubsystem>();
}
