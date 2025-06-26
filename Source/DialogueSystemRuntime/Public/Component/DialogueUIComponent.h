// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "DialogueUIComponent.generated.h"

class IDialogueSelectionContainerWidget;
class UDialogueGraphNode;
class UDialogueSceneNode;
class IRuntimeDialogueGraphVisitor;
class IDialogueSubtitleWidget;


USTRUCT(BlueprintType)
struct FSubtitleParam
{
    GENERATED_BODY()

    enum class EType:uint8
    {
        WithCharacterName, NoCharacterName, None
    };

    FSubtitleParam() = default;

    static TSharedPtr<FSubtitleParam> CreateSubtitleWithCharacterName(const FString& SpeakerName,
                                                                      const FString& DialogueLine);

    static TSharedPtr<FSubtitleParam> CreateSubtitle(const FString& DialogueLine);

    UPROPERTY()
    FString SpeakerName;

    UPROPERTY()
    FString DialogueLine;

    friend bool operator==(const FSubtitleParam& Lhs,
                           const FSubtitleParam& RHS);

    friend bool operator!=(const FSubtitleParam& Lhs,
                           const FSubtitleParam& RHS);
    EType GetType() const;

    FSubtitleParam(const EType Type,
                   const FString& SpeakerName,
                   const FString& DialogueLine)
        : SpeakerName(SpeakerName),
          DialogueLine(DialogueLine),
          Type(Type)
    {
    }

private:
    EType Type;
};

/// <summary>
/// UDialogueUIComponent는 언리얼 엔진의 UActorComponent를 상속받아 대화 시스템에서 자막(Subtitle) UI를 관리하는 컴포넌트입니다.
/// <br>
/// 이 컴포넌트는 캐릭터의 대화 자막을 위젯을 통해 표시하고, 자막의 텍스트 설정, 표시 및 숨김과 같은 기능을 제공합니다.
/// <br>
/// Blueprint에서 사용할 수 있도록 여러 메서드가 UFUNCTION(BlueprintCallable)로 노출되어 있으며,
/// SubtitleWidget과 SubtitleWidgetClass를 통해 자막 위젯의 인스턴스와 클래스를 관리합니다.
/// </summary>
UCLASS(ClassGroup=(Dialogue), meta=(BlueprintSpawnableComponent))
class DIALOGUESYSTEMRUNTIME_API UDialogueUIComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UDialogueUIComponent();

public: // 자막 UI

    /// <summary>
    /// 표시할 자막을 설정합니다.
    /// </summary>
    /// <param name="InSubtitle">설정할 자막</param>
    void SetSubtitle(const TSharedPtr<FSubtitleParam>& InSubtitle);

    void EmptySubtitle();

    /// <summary>
    /// 현재 자막을 반환합니다.
    /// </summary>
    /// <returns>현재 자막</returns>
    UFUNCTION(BlueprintCallable)
    FString GetSubtitleText() const;

    /// <summary>
    /// 자막을 표시합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable)
    void ShowSubtitle() const;

    /// <summary>
    /// 자막을 숨깁니다.
    /// </summary>
    UFUNCTION(BlueprintCallable)
    void HideSubtitle() const;

    /// <summary>
    /// 현재 대화하는 캐릭터의 이름을 반환합니다.
    /// </summary>
    /// <returns>현재 대화하는 캐릭터의 이름</returns>
    UFUNCTION(BlueprintCallable)
    FString GetCurrentSpeakerString() const;

    /// <summary>
    /// 현재 자막의 텍스트를 반환합니다.
    /// </summary>
    /// <returns>현재 자막의 텍스트</returns>
    UFUNCTION(BlueprintCallable)
    FString GetCurrentDialogueLine() const;

public: // 선택지 UI
    UFUNCTION(BlueprintCallable)
    void SetSelectionItem(const TArray<UDialogueSelectionItem*>& SelectionItems) const;

    UFUNCTION(BlueprintCallable)
    void ShowSelectionUI() const;

    UFUNCTION(BlueprintCallable)
    void HideSelectionUI() const;

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

private:
    UFUNCTION(BlueprintCallable)
    void InvalidateSelectionItems() const;

    /// <summary>
    /// 말하는 캐릭터와 자막을 설정합니다.
    /// </summary>
    /// <param name="NameOfSpeakingCharacter">말하는 캐릭터의 이름</param>
    /// <param name="SubtitleText">자막으로 설정할 텍스트</param>
    void SetSpeakingCharacterAndSubtitleText(const FString& NameOfSpeakingCharacter,
                                             const FString& SubtitleText) const;

    /// <summary>
    /// 자막을 설정합니다.
    /// </summary>
    /// <param name="SubtitleText">자막으로 설정할 텍스트</param>
    UFUNCTION(BlueprintCallable)
    void SetSubtitleTextOnly(FString SubtitleText) const;

public:
    UUserWidget* GetRootWidget();

private:
    UPROPERTY(VisibleAnywhere)
    UUserWidget* RootWidget;

    UPROPERTY(VisibleAnywhere)
    TScriptInterface<IDialogueSubtitleWidget> SubtitleWidget;

    UPROPERTY(VisibleAnywhere)
    TScriptInterface<IDialogueSelectionContainerWidget> SelectionWidget;

    UPROPERTY(EditAnywhere)
    TSubclassOf<UUserWidget> RootWidgetClass;

    TSharedPtr<FSubtitleParam> CurrentSubtitle;
};
