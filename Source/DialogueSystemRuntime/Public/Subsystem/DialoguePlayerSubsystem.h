// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DialoguePlayerSubsystem.generated.h"


class UDialogueGraph;
class UListView;
class UDialoguePlayerInstancePool;
class ULevelSequencePlayerHolder;
class ULevelSequence;
class UDialoguePlayerInstance;

UCLASS(Blueprintable)
class DIALOGUESYSTEMRUNTIME_API UDialoguePlayerRuntimeSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    UFUNCTION(BlueprintCallable)
    class UDialogueRuntimePlayer* CreateDialoguePlayerFromDialogueGraph(UObject* ContextObject,
                                                                        const UDialogueGraph* DialogueGraph);

    // UFUNCTION(BlueprintCallable)
    // class UAlternativeDialoguePlayer* CreateDialoguePlayerFromDialogueGraphAsset(const UDialogueGraphAsset* DialogueGraphAsset);

    UFUNCTION(BlueprintCallable)
    ULevelSequencePlayerHolder* RequestNewPlayerInstance() const;

private:
    UPROPERTY()
    UDialoguePlayerInstancePool* PlayerResourcePool;
};

// 대화와 관련된 UI(자막, 선택지 , 셀렉터)를 관리하는 서브시스템
UCLASS(Blueprintable)
class DIALOGUESYSTEMRUNTIME_API UDialogueUISubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
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

// // 대사 Text를 관리하는 서브시스템
// // 에디터 타임
// //  1. Guid를 참조하는 에셋 목록 탐색
// //  2. Guid에 해당하는 Text반환
// UCLASS()
// class DIALOGUESYSTEMRUNTIME_API UDialogueEditorTimeTextSubsystem : public UEditorSubsystem
// {
//     GENERATED_BODY()
// };
