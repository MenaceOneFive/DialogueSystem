// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DialoguePlayerSubsystem.generated.h"

class UDialogueGraph;
class ULevelSequence;
class UDialoguePlayerInstance;

/// 대화 재생을 담당하는 서브시스템
UCLASS()
class DIALOGUESYSTEMRUNTIME_API UDialoguePlayerSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category="DialogueSubsystem|Player")
    void StartDialogue(ULevelSequence* MasterLevelSequence,
                       const UDialogueGraph* InDialogueGraph);

    UFUNCTION(BlueprintCallable, Category="DialogueSubsystem|Player")
    void StartDialogue2(ULevelSequence* MasterLevelSequence,
                        TArray<ULevelSequence*> LevelSequences);

    UFUNCTION(BlueprintCallable, Category="DialogueSubsystem|Player")
    void StopDialogue() const;

    UFUNCTION(BlueprintCallable, Category="DialogueSubsystem|Player")
    void RemoveDialogue();

    virtual void Deinitialize() override
    {
        UGameInstanceSubsystem::Deinitialize();
    };

private:
    UPROPERTY()
    UDialoguePlayerInstance* DialogueInstance;
};

// 대화와 관련된 UI(자막, 선택지 , 셀렉터)를 관리하는 서브시스템
UCLASS()
class DIALOGUESYSTEMRUNTIME_API UDialogueUISubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
};

// 대사 Text를 관리하는 서브시스템
// 런타임
//  1. 대사 에셋을 로딩
//  2. Guid에 해당하는 Text반환
UCLASS()
class DIALOGUESYSTEMRUNTIME_API UDialogueRuntimeTextSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
};

// 대사 Text를 관리하는 서브시스템
// 에디터 타임
//  1. Guid를 참조하는 에셋 목록 탐색
//  2. Guid에 해당하는 Text반환
UCLASS()
class DIALOGUESYSTEMRUNTIME_API UDialogueEditorTimeTextSubsystem : public UEditorSubsystem
{
    GENERATED_BODY()
};
