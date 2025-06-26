// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/Slate/PropertyEditor/DialogueLineGenerationSteps.h"

#include "Widgets/Text/SMultiLineEditableText.h"

#define LOCTEXT_NAMESPACE "ChatBox"

void SChatBox::Construct(const FArguments& Args)
{
    OnChatResponse     = Args._OnChatResponse;
    OnRequestSubmitted = Args._OnRequestSubmitted;

    SAssignNew(ScrollBar, SScrollBar);

    ChildSlot
    [
#pragma region 헤더
        SNew(SVerticalBox)
        + SVerticalBox::Slot()
        .AutoHeight()
        [
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
            .AutoWidth()
            [
                SNew(STextBlock)
                .Text_Lambda([this]
                {
                    return FText::Format(LOCTEXT("Attempts", "{0}번째 수정"), this->NthAttempt);
                })
            ]
            + SHorizontalBox::Slot()
            .FillWidth(1.f)
            [
                SNew(SSpacer)
            ]
            + SHorizontalBox::Slot()
            .AutoWidth()
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot()
                [
                    SNew(SButton)
                    .Text(LOCTEXT("Prev", "-"))
                    .OnClicked_Raw(this, &SChatBox::MoveToPrevAttempt)
                ]
                + SHorizontalBox::Slot()
                [
                    SNew(SButton)
                    .Text(LOCTEXT("Next", "+"))
                    .OnClicked_Raw(this, &SChatBox::MoveToNextAttempt)
                ]
            ]
        ]
#pragma endregion
#pragma region 출력
        + SVerticalBox::Slot()
        [
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
            .FillWidth(1.f)
            [
                SNew(SScrollBox)
                .Orientation(Orient_Vertical)
                .ExternalScrollbar(ScrollBar)
                + SScrollBox::Slot()
                [
                    SAssignNew(ChatResponse, SMultiLineEditableText)
                    .HintText(LOCTEXT("HintMessage_Output", "생성된 결과는 여기에 표시됩니다."))
                    .AutoWrapText(true)
                    .IsReadOnly(true)
                ]
            ]
            + SHorizontalBox::Slot()
            .AutoWidth()
            [
                ScrollBar.ToSharedRef()
            ]
        ]
#pragma endregion
#pragma region 입력
        + SVerticalBox::Slot()
        [
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
            .FillWidth(9.f)
            [
                SAssignNew(ChatBoxInput, SMultiLineEditableText)
                .HintText(LOCTEXT("HintMessage_Input", "앞서 수집된 데이터 외에 AI가 참고해야할 추가사항을 입력해주세요."))
                .AutoWrapText(true)
            ]
            + SHorizontalBox::Slot()
            .FillWidth(1.f)
            .HAlign(HAlign_Center)
            .VAlign(VAlign_Fill)
            [
                SAssignNew(SubmitButton, SButton)
                .HAlign(HAlign_Center)
                .VAlign(VAlign_Center)
                .OnClicked_Lambda([this]
                {
                    ensure(OnRequestSubmitted.IsBound());
                    if ( OnRequestSubmitted.IsBound() )
                    {
                        OnRequestSubmitted.Execute(ChatBoxInput->GetText());
                        ChatBoxInput->SetText(FText::FromString(""));
                    }
                    return FReply::Handled();
                })
                [
                    SNew(STextBlock)
                    .Text(LOCTEXT("Submit", "전송"))
                ]
            ]
        ]
#pragma endregion
    ];
    const FString Msg =
            "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.";
    // ChatResponse->SetText(FText::FromString(Msg));
}

FReply SChatBox::MoveToPrevAttempt() const
{
    return FReply::Handled();
}

FReply SChatBox::MoveToNextAttempt() const
{
    return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
