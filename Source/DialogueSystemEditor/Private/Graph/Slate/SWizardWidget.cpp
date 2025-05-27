// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/Slate/SWizardWidget.h"
#include "Widgets/Layout/SWidgetSwitcher.h"

#define LOCTEXT_NAMESPACE "GenerateDialogueLine"

void SWizardWidget::Construct(const FArguments& Arguments,
                              TArray<FWizardStepTuple> InWidgetToAdd)
{
    SAssignNew(WizardStepSwitcher, SWidgetSwitcher);

    Content = SNew(SVerticalBox)
        + SVerticalBox::Slot()
        .FillHeight(1.0f)
        .Padding(0, 0, 0, 10)
        [
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
            .FillWidth(1.0f)
            [
                SAssignNew(StepIndicator, STextBlock)
            ]
        ]

        + SVerticalBox::Slot()
        .FillHeight(8.0f)
        [
            WizardStepSwitcher.ToSharedRef()
        ]

        + SVerticalBox::Slot()
        .FillHeight(1.0f)
        [
            SNew(SHorizontalBox)

            // 이전 버튼
            + SHorizontalBox::Slot()
            .AutoWidth()
            .VAlign(VAlign_Fill)
            [
                SAssignNew(PrevButton, SButton)
                .HAlign(HAlign_Center)
                .VAlign(VAlign_Center)
                .Text(LOCTEXT("Prev", "이전 단계"))
                .OnClicked(FOnClicked::CreateRaw(this, &SWizardWidget::OnPrevClicked))
                .Visibility(EVisibility::Hidden)
            ]
            + SHorizontalBox::Slot()
            .FillWidth(1.0f)
            [
                SNew(SSpacer)
            ]

            // 다음 버튼
            + SHorizontalBox::Slot()
            .AutoWidth()
            .VAlign(VAlign_Fill)
            [
                SAssignNew(NextButton, SButton)
                .HAlign(HAlign_Center)
                .VAlign(VAlign_Center)
                .Text(LOCTEXT("Next", "다음 단계"))
                .OnClicked(FOnClicked::CreateRaw(this, &SWizardWidget::OnNextClicked))
            ]
        ];

    WidgetToAdd = InWidgetToAdd;

    for ( const auto [StepWidget, StepName] : InWidgetToAdd )
    {
        WizardStepSwitcher->AddSlot()
        [
            StepWidget
        ];
    }

    UpdateStepIndicatorText();

    ChildSlot
    [
        Content.ToSharedRef()
    ];
}

FReply SWizardWidget::OnPrevClicked() const
{
    const int32 CurrentIndex = WizardStepSwitcher->GetActiveWidgetIndex();
    const int32 NumOfWidget  = WizardStepSwitcher->GetNumWidgets();
    if ( Content.IsValid() )
    {
        if ( CurrentIndex - 1 >= 0 )
        {
            WizardStepSwitcher->SetActiveWidgetIndex(CurrentIndex - 1);
            if ( CurrentIndex - 1 == 0 )
            {
                PrevButton->SetVisibility(EVisibility::Hidden);
            }
        }
        if ( CurrentIndex - 1 < NumOfWidget )
        {
            auto Button = &NextButton->GetContent().Get();
            static_cast<STextBlock*>(Button)->SetText(LOCTEXT("Next", "다음 단계"));
        }
        UpdateStepIndicatorText();
    }
    return FReply::Handled();
}

FReply SWizardWidget::OnNextClicked() const
{
    const int32 CurrentIndex = WizardStepSwitcher->GetActiveWidgetIndex();
    const int32 NumOfWidget  = WizardStepSwitcher->GetNumWidgets();
    if ( Content.IsValid() )
    {
        const int32 NextIndex = CurrentIndex + 1;
        PrevButton->SetVisibility(EVisibility::Visible);
        if ( NextIndex == NumOfWidget )
        {
            FSlateApplication::Get().FindWidgetWindow(Content.ToSharedRef())->DestroyWindowImmediately();
            return FReply::Handled();
        }
        if ( NextIndex < NumOfWidget )
        {
            WizardStepSwitcher->SetActiveWidgetIndex(NextIndex);
        }
        if ( NextIndex + 1 == NumOfWidget )
        {
            auto Button = &NextButton->GetContent().Get();
            static_cast<STextBlock*>(Button)->SetText(LOCTEXT("Done", "완료"));
        }
        UpdateStepIndicatorText();
    }
    return FReply::Handled();
}

void SWizardWidget::UpdateStepIndicatorText() const
{
    const int32 Idx = WizardStepSwitcher->GetActiveWidgetIndex();
    StepIndicator->SetText(FText::Format(LOCTEXT("Steps", "단계 {0}/{1} : {2}"),
                                         FText::AsNumber(WizardStepSwitcher->GetActiveWidgetIndex() + 1),
                                         FText::AsNumber(WizardStepSwitcher->GetNumWidgets()),
                                         WidgetToAdd[Idx].Value));
}


#undef LOCTEXT_NAMESPACE
