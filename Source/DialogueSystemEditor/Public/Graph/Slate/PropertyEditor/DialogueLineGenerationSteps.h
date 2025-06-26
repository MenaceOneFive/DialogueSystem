// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


class STextScroller;
class SMultiLineEditableText;

DECLARE_DELEGATE_OneParam(FOnChatResponse, FText)
DECLARE_DELEGATE_OneParam(FOnRequestSubmitted, const FText&)

class SChatBox final : public SCompoundWidget
{
    SLATE_BEGIN_ARGS(SChatBox)
        {
        }

        SLATE_EVENT(FOnChatResponse, OnChatResponse)
        SLATE_EVENT(FOnRequestSubmitted, OnRequestSubmitted)
    SLATE_END_ARGS()

public:
    void Construct(const FArguments& Args);

    TSharedPtr<SMultiLineEditableText> ChatResponse;
    TSharedPtr<SMultiLineEditableText> ChatBoxInput;

private:
    int32 NthAttempt = 1;
    FOnChatResponse OnChatResponse;
    FOnRequestSubmitted OnRequestSubmitted;

    TSharedPtr<SButton> SubmitButton;
    TSharedPtr<SScrollBar> ScrollBar;

    TArray<TTuple<FText, FText>> ChatHistory;
    FReply MoveToPrevAttempt() const;
    FReply MoveToNextAttempt() const;
};
