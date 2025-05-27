// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueSceneTrack.h"
#include "Section/PlayerControlSection.h"
#include "UObject/Object.h"
#include "PlayerControlTrack.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGUESYSTEMRUNTIME_API UPlayerControlTrack : public UDialogueSceneTrack
{
	GENERATED_BODY()

public:
	UPlayerControlTrack(const FObjectInitializer& Initializer): UDialogueSceneTrack(Initializer)
	{
		SetDescription("사용자가 대화의 흐름을 제어하는 데 사용합니다.");
		Initialize(TEXT("플레이어 컨트롤 트랙"), true, UPlayerControlSection::StaticClass());
	} ;
};
