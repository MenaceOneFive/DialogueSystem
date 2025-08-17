// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UObject/Interface.h"
#include "DialoguePlayerUIRoot.generated.h"

class IDialogueSubtitleWidget;
class IDialogueSelectionContainerWidget;

USTRUCT(BlueprintType)
struct FDialogueUIKeyEvent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FKey Key;

    UPROPERTY(BlueprintReadWrite)
    FName EventName;

    FSimpleDelegate OnKeyPressed;
    FSimpleDelegate OnKeyReleased;
};

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

DECLARE_DYNAMIC_DELEGATE_OneParam(FDialoguePlayerUIMouseEvent, const FPointerEvent&, InMouseEvent);
DECLARE_DYNAMIC_DELEGATE(FDialoguePlayerKeyboardEvent);

// 키/마우스 이벤트를 C++에서 멀티캐스트로 처리하기 위한 네이티브 델리게이트 선언
// 블루프린트용 동적 델리게이트(FDialoguePlayerUIMouseEvent, FDialoguePlayerKeyboardEvent)는
// 바인드 API의 인자로 받아 내부적으로 네이티브 멀티캐스트에 연결된다.
DECLARE_MULTICAST_DELEGATE_OneParam(FDialoguePlayerUIMouseEventNative, const FPointerEvent&);

UCLASS(Blueprintable)
class UDialoguePlayerUIRootImpl : public UUserWidget,
                                  public IDialoguePlayerUIRoot
{
    GENERATED_BODY()

public:
    virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
    virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
    /// <summary>
    /// 키 다운 이벤트 바인드(블루프린트에서 사용).<br>
    /// 해당 키가 눌렸을 때 전달된 델리게이트를 실행한다.
    /// <param name="Key">바인드할 키</param>
    /// <param name="InEvent">호출될 동적 델리게이트(파라미터 없음)</param>
    /// </summary>
    UFUNCTION(BlueprintCallable)
    void BindKeyDownEvent(FKey Key, FDialoguePlayerKeyboardEvent InEvent);

    /// <summary>
    /// 키 업 이벤트 바인드(블루프린트에서 사용).<br>
    /// 해당 키가 올라갔을 때 전달된 델리게이트를 실행한다.
    /// <param name="Key">바인드할 키</param>
    /// <param name="InEvent">호출될 동적 델리게이트(파라미터 없음)</param>
    /// </summary>
    UFUNCTION(BlueprintCallable)
    void BindKeyUpEvent(FKey Key, FDialoguePlayerKeyboardEvent InEvent);

    /// <summary>
    /// 마우스 버튼 다운 이벤트 바인드(블루프린트에서 사용).<br>
    /// 해당 버튼이 눌렸을 때 전달된 델리게이트를 실행한다.
    /// <param name="MouseButton">바인드할 마우스 버튼</param>
    /// <param name="InEvent">호출될 동적 델리게이트(파라미터: InMouseEvent)</param>
    /// </summary>
    UFUNCTION(BlueprintCallable)
    void BindMouseButtonDownEvent(FKey MouseButton, FDialoguePlayerUIMouseEvent InEvent);

    /// <summary>
    /// 마우스 버튼 업 이벤트 바인드(블루프린트에서 사용).<br>
    /// 해당 버튼이 올라갔을 때 전달된 델리게이트를 실행한다.
    /// <param name="MouseButton">바인드할 마우스 버튼</param>
    /// <param name="InEvent">호출될 동적 델리게이트(파라미터: InMouseEvent)</param>
    /// </summary>
    UFUNCTION(BlueprintCallable)
    void BindMouseButtonUpEvent(FKey MouseButton, FDialoguePlayerUIMouseEvent InEvent);

    /// <summary>
    /// 마우스 더블 클릭 이벤트 바인드(블루프린트에서 사용).<br>
    /// 해당 버튼이 더블 클릭되었을 때 전달된 델리게이트를 실행한다.
    /// <param name="MouseButton">바인드할 마우스 버튼</param>
    /// <param name="InEvent">호출될 동적 델리게이트(파라미터: InMouseEvent)</param>
    /// </summary>
    UFUNCTION(BlueprintCallable)
    void BindMouseButtonDoubleClickEvent(FKey MouseButton, FDialoguePlayerUIMouseEvent InEvent);

    /// <summary>
    /// 특정 키의 키 다운 바인딩 전부 해제.<br>
    /// 해당 키에 바인드된 모든 콜백이 제거된다.
    /// <param name="Key">해제할 키</param>
    /// </summary>
    UFUNCTION(BlueprintCallable)
    void UnbindKeyDownEvent(FKey Key);

    /// <summary>
    /// 특정 키의 키 업 바인딩 전부 해제.<br>
    /// 해당 키에 바인드된 모든 콜백이 제거된다.
    /// <param name="Key">해제할 키</param>
    /// </summary>
    UFUNCTION(BlueprintCallable)
    void UnbindKeyUpEvent(FKey Key);

    /// <summary>
    /// 특정 마우스 버튼의 다운 바인딩 전부 해제.
    /// <param name="MouseButton">해제할 마우스 버튼</param>
    /// </summary>
    UFUNCTION(BlueprintCallable)
    void UnbindMouseButtonDownEvent(FKey MouseButton);

    /// <summary>
    /// 특정 마우스 버튼의 업 바인딩 전부 해제.
    /// <param name="MouseButton">해제할 마우스 버튼</param>
    /// </summary>
    UFUNCTION(BlueprintCallable)
    void UnbindMouseButtonUpEvent(FKey MouseButton);

    /// <summary>
    /// 특정 마우스 버튼의 더블 클릭 바인딩 전부 해제.
    /// <param name="MouseButton">해제할 마우스 버튼</param>
    /// </summary>
    UFUNCTION(BlueprintCallable)
    void UnbindMouseButtonDoubleClickEvent(FKey MouseButton);

    /// <summary>
    /// 모든 키/마우스 바인딩을 초기화한다.
    /// </summary>
    UFUNCTION(BlueprintCallable)
    void ClearAllBindings();

private:
    // 키 이벤트: 파라미터 없음
    TMap<FKey, FSimpleMulticastDelegate> KeyToKeyDownEvent;
    TMap<FKey, FSimpleMulticastDelegate> KeyToKeyUpEvent;

    // 마우스 이벤트: FPointerEvent를 파라미터로 전달
    TMap<FKey, FDialoguePlayerUIMouseEventNative> KeyToMouseButtonUpEvent;
    TMap<FKey, FDialoguePlayerUIMouseEventNative> KeyToMouseButtonDownEvent;
    TMap<FKey, FDialoguePlayerUIMouseEventNative> KeyToMouseButtonDoubleClickEvent;
};
