// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UObject/Interface.h"
#include "DialogueSelectionItemWidget.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UDialogueSelectionItemWidget : public UInterface
{
    GENERATED_BODY()
};

/**
 * 
 */
class DIALOGUESYSTEMRUNTIME_API IDialogueSelectionItemWidget
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void SetupSelectionItem(const UDialogueSelectionItem* SelectionItem);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    bool CanSelectThisItem();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void EnableThisItem();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void DisableThisItem();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void ResetThisItem();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void ShowThisItem();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void HideThisItem();
};

UCLASS(Blueprintable)
class UDialogueSelectionItemWidgetImpl : public UUserWidget,
                                         public IDialogueSelectionItemWidget
{
    GENERATED_BODY()
};
