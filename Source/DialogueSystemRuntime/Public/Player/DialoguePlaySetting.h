// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialoguePlaySetting.generated.h"

UENUM(BlueprintType)
enum class EFocusMode:uint8
{
    UI UMETA(Tooltip="UI가 포커스를 가져가고 키보드 마우스 입력을 관리합니다."),
    Game UMETA(Tooltip="게임이 포커스를 가져가고 키보드 마우스 입력을 관리합니다.")
};

UENUM(BlueprintType)
enum class EStateAfterSequencePlay:uint8
{
    Keep UMETA(Tooltip="재생으로 인한 상태변화를 유지합니다."),
    Reset UMETA(Tooltip="시퀀스 재생 전의 상태로 되돌립니다."),
};

/// <summary>
/// 시퀀스를 재생하는 노드의 공통 설정을 담고 있습니다.
/// </summary>
USTRUCT(BlueprintType)
struct DIALOGUESYSTEMRUNTIME_API FSequencePlaySetting
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EStateAfterSequencePlay StateAfterSequencePlay;

    // 스킵이 가능한 노드 타입인 경우 이 노드가 True이면 스킵할 수 있다
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCanSkipThisNode = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StartTimeOfSequence = 0.f;

    // 재생중 플레이어의 이동을 막아야 하는가?
    // 이걸 false로 바꿔야 하면 한번 더 생각해볼 것
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bShouldBlockPlayerMovementDuringDialogue = true;

    // 재생중 플레이어의 카메라 화전을 막아야 하는가?
    // 이걸 false로 바꿔야 하면 한번 더 생각해볼 것
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bShouldBlockPlayerCameraMovementDuringDialogue = true;
};

/// <summary>
/// 모든 유형의 노드가 설정할 수 있는 설정입니다.
/// </summary>
USTRUCT(BlueprintType)
struct DIALOGUESYSTEMRUNTIME_API FDialogueSetting
{
    GENERATED_BODY()
    // 누가 포커스를 가지고 입력에 대한 책임을 져야 하는가?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EFocusMode EFocusMode = EFocusMode::UI;

    // 커서를 보여줘야 하는가?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bShouldShowCursor = false;

    // 여러번 재생할 수 있는가?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCanVisitMultipleTimes = true;

    // 만약 CanVisitMultipleTimes가 참이면 재생할 수 있는 횟수
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    uint8 PlayableTime = std::numeric_limits<uint8>::max();
};
