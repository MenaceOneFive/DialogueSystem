// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class SMultiLineEditableTextBox;
constexpr static uint32 PromptBufferSize = 10485760;

class FLoreProvider
{
public:
    virtual ~FLoreProvider() = default;

    virtual FString GetLoreTitle() const = 0;
    virtual FString GetLoreContent() const = 0;

    virtual FString ToString() const
    {
        TStringBuilder<PromptBufferSize> StringBuilder;
        StringBuilder.Append("##");
        StringBuilder.Append(GetLoreTitle());
        StringBuilder.Append("\n");
        StringBuilder.Append(GetLoreContent());
        StringBuilder.Append("\n");
        return StringBuilder.ToString();
    };
};

DECLARE_DELEGATE_OneParam(FOnGenerateButtonClicked, FText)

/// <summary>
/// 생성형 AI에게 제공할 프롬프트를 입력하고 전송하는데 사용할 수 있는 위젯
/// - TextArea
/// - Button
/// - Text로 구성되어 있음
/// </summary>
class DIALOGUESYSTEMEDITOR_API SGenAIPromptWidget final : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SGenAIPromptWidget)
        {
        }

        SLATE_ARGUMENT(FOnGenerateButtonClicked, OnGenerateButtonClicked) // 전송 버튼을 누른 경우
        SLATE_ARGUMENT(FOnTextCommitted, OnUserPromptCommitted)
        SLATE_ARGUMENT(FString, UserPrompt)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);
    FReply OnGenerateButtonClicked() const;

private:
    TSharedPtr<SMultiLineEditableTextBox> UserPromptTextBox;
    FOnGenerateButtonClicked OnGenerateButtonClickedDelegate;
    FOnTextCommitted OnUserPromptCommitted;
};
