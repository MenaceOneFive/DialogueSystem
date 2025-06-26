// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UObject/Interface.h"
#include "DialoguePlayerUIRoot.generated.h"

class IDialogueSubtitleWidget;
class IDialogueSelectionContainerWidget;
// This class does not need to be modified.
UINTERFACE()
class UDialoguePlayerUIRoot : public UInterface
{
    GENERATED_BODY()
};

/**
 * 
 */
class DIALOGUESYSTEMRUNTIME_API IDialoguePlayerUIRoot
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void InitializeSubtitleWidget();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    TScriptInterface<IDialogueSubtitleWidget> GetSubtitleWidget();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void InitializeSelectionWidget();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    TScriptInterface<IDialogueSelectionContainerWidget> GetSelectionWidget();
};

UCLASS(Blueprintable)
class UDialoguePlayerUIRootImpl : public UUserWidget,
                                  public IDialoguePlayerUIRoot
{
    GENERATED_BODY()

public:
};
