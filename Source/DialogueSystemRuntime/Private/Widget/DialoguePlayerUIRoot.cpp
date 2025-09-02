// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/DialoguePlayerUIRoot.h"


// Add default functionality here for any IDialoguePlayerUIRoot functions that are not pure virtual.
FReply UDialoguePlayerUIRootImpl::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    // 입력된 키를 조회한다
    const FKey Key = InKeyEvent.GetKey();

    // 키 다운 바인딩이 존재하면 브로드캐스트하고 처리 완료를 반환한다
    if (FSimpleMulticastDelegate* Delegate = KeyToKeyDownEvent.Find(Key))
    {
        if (Delegate->IsBound())
        {
            // 파라미터 없는 키 다운 이벤트 브로드캐스트
            Delegate->Broadcast();
            return FReply::Handled();
        }
    }

    // 바인딩이 없으면 부모 기본 동작을 수행한다
    return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

FReply UDialoguePlayerUIRootImpl::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    // 입력된 키를 조회한다
    const FKey Key = InKeyEvent.GetKey();

    // 키 업 바인딩이 존재하면 브로드캐스트하고 처리 완료를 반환한다
    if (FSimpleMulticastDelegate* Delegate = KeyToKeyUpEvent.Find(Key))
    {
        if (Delegate->IsBound())
        {
            // 파라미터 없는 키 업 이벤트 브로드캐스트
            Delegate->Broadcast();
            return FReply::Handled();
        }
    }

    // 바인딩이 없으면 부모 기본 동작을 수행한다
    return Super::NativeOnKeyUp(InGeometry, InKeyEvent);
}

FReply UDialoguePlayerUIRootImpl::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    // 효과를 일으킨 마우스 버튼 키를 조회한다
    const FKey ButtonKey = InMouseEvent.GetEffectingButton();

    // 마우스 버튼 다운 바인딩이 존재하면 브로드캐스트하고 처리 완료를 반환한다
    if (FDialoguePlayerUIMouseEventNative* Delegate = KeyToMouseButtonDownEvent.Find(ButtonKey))
    {
        if (Delegate->IsBound())
        {
            Delegate->Broadcast(InMouseEvent);
            return FReply::Handled();
        }
    }

    // 바인딩이 없으면 부모 기본 동작을 수행한다
    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UDialoguePlayerUIRootImpl::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    // 효과를 일으킨 마우스 버튼 키를 조회한다
    const FKey ButtonKey = InMouseEvent.GetEffectingButton();

    // 마우스 버튼 업 바인딩이 존재하면 브로드캐스트하고 처리 완료를 반환한다
    if (FDialoguePlayerUIMouseEventNative* Delegate = KeyToMouseButtonUpEvent.Find(ButtonKey))
    {
        if (Delegate->IsBound())
        {
            Delegate->Broadcast(InMouseEvent);
            return FReply::Handled();
        }
    }

    // 바인딩이 없으면 부모 기본 동작을 수행한다
    return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

FReply UDialoguePlayerUIRootImpl::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    // 효과를 일으킨 마우스 버튼 키를 조회한다
    const FKey ButtonKey = InMouseEvent.GetEffectingButton();

    // 마우스 버튼 더블 클릭 바인딩이 존재하면 브로드캐스트하고 처리 완료를 반환한다
    if (FDialoguePlayerUIMouseEventNative* Delegate = KeyToMouseButtonDoubleClickEvent.Find(ButtonKey))
    {
        if (Delegate->IsBound())
        {
            Delegate->Broadcast(InMouseEvent);
            return FReply::Handled();
        }
    }

    // 바인딩이 없으면 부모 기본 동작을 수행한다
    return Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
}

// ========================= 바인드/언바인드/초기화 API 구현 =========================

void UDialoguePlayerUIRootImpl::BindKeyDownEvent(FKey Key, FDialoguePlayerKeyboardEvent InEvent)
{
    // 해당 키의 멀티캐스트 델리게이트를 얻거나 생성한다
    FSimpleMulticastDelegate& Delegate = KeyToKeyDownEvent.FindOrAdd(Key);

    // 블루프린트 동적 델리게이트를 호출하는 람다를 추가한다
    Delegate.AddLambda([InEvent]() mutable
    {
        // 파라미터 없는 동적 델리게이트 실행
        (void)InEvent.ExecuteIfBound();
    });

}

void UDialoguePlayerUIRootImpl::BindKeyUpEvent(FKey Key, FDialoguePlayerKeyboardEvent InEvent)
{
    // 해당 키의 멀티캐스트 델리게이트를 얻거나 생성한다
    FSimpleMulticastDelegate& Delegate = KeyToKeyUpEvent.FindOrAdd(Key);

    // 블루프린트 동적 델리게이트를 호출하는 람다를 추가한다
    Delegate.AddLambda([InEvent]() mutable
    {
        (void)InEvent.ExecuteIfBound();
    });

}

void UDialoguePlayerUIRootImpl::BindMouseButtonDownEvent(FKey MouseButton, FDialoguePlayerUIMouseEvent InEvent)
{
    // 해당 마우스 버튼의 멀티캐스트 델리게이트를 얻거나 생성한다
    FDialoguePlayerUIMouseEventNative& Delegate = KeyToMouseButtonDownEvent.FindOrAdd(MouseButton);

    // 블루프린트 동적 델리게이트를 호출하는 람다를 추가한다
    Delegate.AddLambda([InEvent](const FPointerEvent& E) mutable
    {
        (void)InEvent.ExecuteIfBound(E);
    });

}

void UDialoguePlayerUIRootImpl::BindMouseButtonUpEvent(FKey MouseButton, FDialoguePlayerUIMouseEvent InEvent)
{
    // 해당 마우스 버튼의 멀티캐스트 델리게이트를 얻거나 생성한다
    FDialoguePlayerUIMouseEventNative& Delegate = KeyToMouseButtonUpEvent.FindOrAdd(MouseButton);

    // 블루프린트 동적 델리게이트를 호출하는 람다를 추가한다
    Delegate.AddLambda([InEvent](const FPointerEvent& E) mutable
    {
        (void)InEvent.ExecuteIfBound(E);
    });

}

void UDialoguePlayerUIRootImpl::BindMouseButtonDoubleClickEvent(FKey MouseButton, FDialoguePlayerUIMouseEvent InEvent)
{
    // 해당 마우스 버튼의 멀티캐스트 델리게이트를 얻거나 생성한다
    FDialoguePlayerUIMouseEventNative& Delegate = KeyToMouseButtonDoubleClickEvent.FindOrAdd(MouseButton);

    // 블루프린트 동적 델리게이트를 호출하는 람다를 추가한다
    Delegate.AddLambda([InEvent](const FPointerEvent& E) mutable
    {
        (void)InEvent.ExecuteIfBound(E);
    });

}

void UDialoguePlayerUIRootImpl::UnbindKeyDownEvent(FKey Key)
{
    // 해당 키의 모든 바인딩을 제거한다
    if (FSimpleMulticastDelegate* Delegate = KeyToKeyDownEvent.Find(Key))
    {
        Delegate->Clear();
    }

}

void UDialoguePlayerUIRootImpl::UnbindKeyUpEvent(FKey Key)
{
    // 해당 키의 모든 바인딩을 제거한다
    if (FSimpleMulticastDelegate* Delegate = KeyToKeyUpEvent.Find(Key))
    {
        Delegate->Clear();
    }

}

void UDialoguePlayerUIRootImpl::UnbindMouseButtonDownEvent(FKey MouseButton)
{
    // 해당 마우스 버튼의 모든 바인딩을 제거한다
    if (FDialoguePlayerUIMouseEventNative* Delegate = KeyToMouseButtonDownEvent.Find(MouseButton))
    {
        Delegate->Clear();
    }

}

void UDialoguePlayerUIRootImpl::UnbindMouseButtonUpEvent(FKey MouseButton)
{
    // 해당 마우스 버튼의 모든 바인딩을 제거한다
    if (FDialoguePlayerUIMouseEventNative* Delegate = KeyToMouseButtonUpEvent.Find(MouseButton))
    {
        Delegate->Clear();
    }

}

void UDialoguePlayerUIRootImpl::UnbindMouseButtonDoubleClickEvent(FKey MouseButton)
{
    // 해당 마우스 버튼의 모든 바인딩을 제거한다
    if (FDialoguePlayerUIMouseEventNative* Delegate = KeyToMouseButtonDoubleClickEvent.Find(MouseButton))
    {
        Delegate->Clear();
    }

}

void UDialoguePlayerUIRootImpl::ClearAllBindings()
{
    // 키 이벤트 모두 초기화
    for (TPair<FKey, FSimpleMulticastDelegate>& Pair : KeyToKeyDownEvent)
    {
        Pair.Value.Clear();
    }
    for (TPair<FKey, FSimpleMulticastDelegate>& Pair : KeyToKeyUpEvent)
    {
        Pair.Value.Clear();
    }

    // 마우스 이벤트 모두 초기화
    for (TPair<FKey, FDialoguePlayerUIMouseEventNative>& Pair : KeyToMouseButtonDownEvent)
    {
        Pair.Value.Clear();
    }
    for (TPair<FKey, FDialoguePlayerUIMouseEventNative>& Pair : KeyToMouseButtonUpEvent)
    {
        Pair.Value.Clear();
    }
    for (TPair<FKey, FDialoguePlayerUIMouseEventNative>& Pair : KeyToMouseButtonDoubleClickEvent)
    {
        Pair.Value.Clear();
    }

    // 맵을 비운다
    KeyToKeyDownEvent.Empty();
    KeyToKeyUpEvent.Empty();
    KeyToMouseButtonDownEvent.Empty();
    KeyToMouseButtonUpEvent.Empty();
    KeyToMouseButtonDoubleClickEvent.Empty();

}
