// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DialogueSystemEvent.generated.h"

class UDialoguePlayerRuntimeSubsystem;

/// <summary>
/// CustomEventNode에 사용하는 CustomEvent
/// </summary>
UCLASS(Blueprintable)
class DIALOGUESYSTEMRUNTIME_API UDialogueSystemEvent : public UObject
{
    GENERATED_BODY()

public:
    /// <summary>
    /// 주의!
    /// 이 메서드를 직접 호출하지 마십시오. DialoguePlayer에서 알아서 실행할 것입니다.
    /// 직접 정의하는 동작의 시작 전에, 오류에 대한 검증을 수행하기 위해 부모에 대한 호출을 수행하십시오 
    /// </summary>
    void StartEvent();

    /// <summary>
    /// 이 메서드를 오버라이드해서 커스텀 이벤트를 구현할 수 있습니다.
    /// </summary>
    UFUNCTION(BlueprintNativeEvent)
    void Execute();

    /// <summary>
    /// 이벤트를 종료합니다.
    /// 사전에 정의된 다음 노드를 방문합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable)
    void EndEvent() const;

    // 다음 노드 방문을 위한 이벤트 (수동조작 X)
    FSimpleDelegate OnVisitNextNode;

    UFUNCTION(BlueprintPure)
    ACharacter* GetCharacter() const;

    UFUNCTION(BlueprintPure)
    UWorld* GetCharacterWorld() const;

    UFUNCTION(BlueprintCallable)
    UDialoguePlayerRuntimeSubsystem* GetDialoguePlayerRuntimeSubsystem() const;

protected:
    UPROPERTY(EditAnywhere)
    FString EventName;
};
