// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/Slate/SPromptWidget.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"


void SGenAIPromptWidget::Construct(const FArguments& InArgs)
{
    OnGenerateButtonClickedDelegate = InArgs._OnGenerateButtonClicked;
    OnUserPromptCommitted           = InArgs._OnUserPromptCommitted;

    check(OnGenerateButtonClickedDelegate.IsBound() && OnUserPromptCommitted.IsBound());

    this->ChildSlot
    [
        SNew(SVerticalBox)
        + SVerticalBox::Slot()
        [
            SAssignNew(UserPromptTextBox, SMultiLineEditableTextBox) // 사용자 지정 프롬프트
            .Text(FText::FromString(InArgs._UserPrompt))
            .OnTextCommitted(OnUserPromptCommitted)
        ]
        + SVerticalBox::Slot()
        [
            SNew(SButton) // 생성 / 전송 버튼
            .OnClicked(FOnClicked::CreateSP(this, &SGenAIPromptWidget::OnGenerateButtonClicked))
        ]
    ];
}

FReply SGenAIPromptWidget::OnGenerateButtonClicked() const
{
    if (OnGenerateButtonClickedDelegate.IsBound())
    {
        // 사용자가 텍스트 박스에 입력한 텍스트를 프롬프트로 출력
        const FText UserPrompt = UserPromptTextBox->GetText();
        OnGenerateButtonClickedDelegate.Execute(UserPrompt);
    }
    return FReply::Handled();
}
