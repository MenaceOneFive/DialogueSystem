// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/SlateWrapperTypes.h"
#include "UObject/Interface.h"
#include "DialogueSubtitleWidget.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UDialogueSubtitleWidget : public UInterface
{
    GENERATED_BODY()
};

class DIALOGUESYSTEMRUNTIME_API IDialogueSubtitleWidget
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void SetSpeakingCharacterAndSubtitleText(const FString& SpeakingCharacterName,
                                             const FString& SubtitleText);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void SetSubtitleText(const FString& SubtitleText);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    FString GetSubtitleText();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void ShowSubtitle();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void HideSubtitle();
};

UCLASS()
class UDialogueSubtitleWidgetImpl : public UUserWidget,
                                    public IDialogueSubtitleWidget
{
    GENERATED_BODY()

public:
};
