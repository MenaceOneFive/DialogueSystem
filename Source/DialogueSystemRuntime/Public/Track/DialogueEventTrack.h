// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueSceneTrack.h"
#include "MovieSceneTrack.h"
#include "Section/DialogueEventSection.h"
#include "DialogueEventTrack.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGUESYSTEMRUNTIME_API UDialogueEventTrack : public UDialogueSceneTrack
{
	GENERATED_BODY()

public:
	UDialogueEventTrack(const FObjectInitializer& Initializer): UDialogueSceneTrack(Initializer)
	{
		SetDescription("현재 대화를 제어하는 데 사용합니다.");
		Initialize(TEXT("대화 이벤트 트랙"), true, UDialogueEventSection::StaticClass());
	} ;
};
