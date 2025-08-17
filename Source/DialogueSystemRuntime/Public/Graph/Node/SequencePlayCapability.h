// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/DialoguePlaySetting.h"
#include "UObject/Interface.h"
#include "SequencePlayCapability.generated.h"

class ULevelSequence;
// This class does not need to be modified.
UINTERFACE()
class USequencePlayCapability : public UInterface
{
    GENERATED_BODY()
};

/// <summary>
/// UDialogueNode중 시퀀스 재생 능력이 있는 노드를 만들고 싶다면(특히 커스텀 노드)
/// 이것을 구현하면 된다.
/// </summary>
class DIALOGUESYSTEMRUNTIME_API ISequencePlayCapability
{
    GENERATED_BODY()

public:
    virtual TSoftObjectPtr<ULevelSequence> GetLevelSequenceToPlay() const = 0;
    virtual FSequencePlaySetting GetSequencePlaySetting() const = 0;
#if WITH_EDITORONLY_DATA
    virtual void SetLevelSequenceToPlay(const TSoftObjectPtr<ULevelSequence>& InLevelSequence) = 0;
    virtual void SetSequencePlaySetting(const FSequencePlaySetting& InSetting) = 0;
#endif
};
