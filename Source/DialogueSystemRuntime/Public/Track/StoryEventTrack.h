// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueSceneTrack.h"
#include "Section/StoryEventSection.h"
#include "StoryEventTrack.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGUESYSTEMRUNTIME_API UStoryEventTrack : public UDialogueSceneTrack
{
	GENERATED_BODY()

public:
	UStoryEventTrack(const FObjectInitializer& Initializer): UDialogueSceneTrack(Initializer)
	{
		SetDescription("게임 전체 스토리에 영향을 주는 경우 사용됩니다.");
		Initialize(TEXT("스토리 이벤트 트랙"), true, UStoryEventSection::StaticClass());
	} ;
};
