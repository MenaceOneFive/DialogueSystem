// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueSystemRuntime/Public/Component/DialogueUIComponent.h"

#include "Blueprint/UserWidget.h"
#include "Graph/Node/DialogueGraphNode.h"
#include "Widget/DialoguePlayerUIRoot.h"
#include "Widget/DialogueSelectionContainerWidget.h"
#include "Widget/DialogueSubtitleWidget.h"


TSharedPtr<FSubtitleParam> FSubtitleParam::CreateSubtitleWithCharacterName(const FString& SpeakerName,
                                                                           const FString& DialogueLine)
{
    return MakeShared<FSubtitleParam>(EType::WithCharacterName, SpeakerName, DialogueLine);
}

TSharedPtr<FSubtitleParam> FSubtitleParam::CreateSubtitle(const FString& DialogueLine)
{
    return MakeShared<FSubtitleParam>(EType::WithCharacterName, "", DialogueLine);
}

FSubtitleParam::EType FSubtitleParam::GetType() const
{
    return Type;
}


// Sets default values for this component's properties
// ReSharper disable once CppPossiblyUninitializedMember
UDialogueUIComponent::UDialogueUIComponent(): CurrentSubtitle()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UDialogueUIComponent::SetSubtitle(const TSharedPtr<FSubtitleParam>& InSubtitle)
{
    if ( !InSubtitle.IsValid() )
    {
        return;
    }

    if ( CurrentSubtitle == InSubtitle )
    {
        return;
    }

    CurrentSubtitle = InSubtitle;

    if ( CurrentSubtitle->GetType() == FSubtitleParam::EType::WithCharacterName )
    {
        SetSpeakingCharacterAndSubtitleText(CurrentSubtitle->SpeakerName, CurrentSubtitle->DialogueLine);
    }

    if ( CurrentSubtitle->GetType() == FSubtitleParam::EType::NoCharacterName )
    {
        SetSubtitleTextOnly(CurrentSubtitle->DialogueLine);
    }

    ShowSubtitle();
}

void UDialogueUIComponent::EmptySubtitle()
{
    CurrentSubtitle = nullptr;
}

// Called when the game starts
void UDialogueUIComponent::BeginPlay()
{
    Super::BeginPlay();

    const auto Pawn = Cast<APawn>(GetOwner());

    const auto PlayerController = Cast<APlayerController>(Pawn->GetController());

    // 위젯을 초기화

    RootWidget = CreateWidget<UUserWidget>(PlayerController, RootWidgetClass);
    RootWidget->AddToPlayerScreen();
    checkf(RootWidget, TEXT("RootWidget의 CreateWidget에 실패했습니다."));

    if ( RootWidget->Implements<UDialoguePlayerUIRoot>() )
    {
        IDialoguePlayerUIRoot::Execute_InitializeSubtitleWidget(RootWidget);
        IDialoguePlayerUIRoot::Execute_InitializeSelectionWidget(RootWidget);

        const auto WidgetInterface = IDialoguePlayerUIRoot::Execute_GetSubtitleWidget(RootWidget);
        SubtitleWidget.SetObject(WidgetInterface.GetObject());
        SubtitleWidget.SetInterface(WidgetInterface.GetInterface());
        checkf(SubtitleWidget.GetObject()->IsValidLowLevel(), TEXT("SubtitleWidget의 CreateWidget에 실패했습니다."));

        const auto SelectionInterface = IDialoguePlayerUIRoot::Execute_GetSelectionWidget(RootWidget);
        SelectionWidget.SetObject(SelectionInterface.GetObject());
        SelectionWidget.SetInterface(SelectionInterface.GetInterface());
        checkf(SelectionWidget.GetObject()->IsValidLowLevel(), TEXT("SelectionWidget의 CreateWidget에 실패했습니다. "));
    }

    HideSubtitle();
}

void UDialogueUIComponent::InvalidateSelectionItems() const
{
    if (SelectionWidget)
    {
        IDialogueSelectionContainerWidget::Execute_ResetAllSelectionItem(SelectionWidget.GetObject());
    }
}

void UDialogueUIComponent::SetSpeakingCharacterAndSubtitleText(const FString& NameOfSpeakingCharacter,
                                                               const FString& SubtitleText) const
{
    if (SubtitleWidget)
    {
        IDialogueSubtitleWidget::Execute_SetSpeakingCharacterAndSubtitleText(SubtitleWidget.GetObject(), NameOfSpeakingCharacter, SubtitleText);
    }
}

void UDialogueUIComponent::SetSubtitleTextOnly(const FString SubtitleText) const
{
    if (SubtitleWidget)
    {
        IDialogueSubtitleWidget::Execute_SetSubtitleText(SubtitleWidget.GetObject(), SubtitleText);
    }
}

UUserWidget* UDialogueUIComponent::GetRootWidget()
{
    return RootWidget;
}

FString UDialogueUIComponent::GetCurrentSpeakerString() const
{
    if ( CurrentSubtitle.IsValid() )
    {
        return CurrentSubtitle->SpeakerName;
    }
    return "";
}

FString UDialogueUIComponent::GetCurrentDialogueLine() const
{
    if ( CurrentSubtitle.IsValid() )
    {
        return CurrentSubtitle->DialogueLine;
    }
    return "";
}

void UDialogueUIComponent::SetSelectionItem(const TArray<UDialogueSelectionItem*>& SelectionItems) const
{
    const int Num = SelectionItems.Num();
    checkf(Num <= 8, TEXT("선택지는 8개를 넘길 수 없습니다."))

    InvalidateSelectionItems();
    
    if (SelectionWidget)
    {
        if ( Num > 0 )
        {
            IDialogueSelectionContainerWidget::Execute_Set_1_30_SelectionItem(SelectionWidget.GetObject(), SelectionItems[0]);
        }
        if ( Num > 1 )
        {
            IDialogueSelectionContainerWidget::Execute_Set_3_00_SelectionItem(SelectionWidget.GetObject(), SelectionItems[1]);
        }
        if ( Num > 2 )
        {
            IDialogueSelectionContainerWidget::Execute_Set_4_30_SelectionItem(SelectionWidget.GetObject(), SelectionItems[2]);
        }
        if ( Num > 3 )
        {
            IDialogueSelectionContainerWidget::Execute_Set_6_00_SelectionItem(SelectionWidget.GetObject(), SelectionItems[3]);
        }
        if ( Num > 4 )
        {
            IDialogueSelectionContainerWidget::Execute_Set_7_30_SelectionItem(SelectionWidget.GetObject(), SelectionItems[4]);
        }
        if ( Num > 5 )
        {
            IDialogueSelectionContainerWidget::Execute_Set_9_00_SelectionItem(SelectionWidget.GetObject(), SelectionItems[4]);
        }
        if ( Num > 6 )
        {
            IDialogueSelectionContainerWidget::Execute_Set_10_30_SelectionItem(SelectionWidget.GetObject(), SelectionItems[5]);
        }
        if ( Num > 7 )
        {
            IDialogueSelectionContainerWidget::Execute_Set_12_00_SelectionItem(SelectionWidget.GetObject(), SelectionItems[6]);
        }
    }
}


void UDialogueUIComponent::ShowSelectionUI() const
{
    if (SelectionWidget)
    {
        IDialogueSelectionContainerWidget::Execute_ShowSelectionWidget(SelectionWidget.GetObject());
    }
}

void UDialogueUIComponent::HideSelectionUI() const
{
    if (SelectionWidget)
    {
        IDialogueSelectionContainerWidget::Execute_HideSelectionWidget(SelectionWidget.GetObject());
    }
}

FString UDialogueUIComponent::GetSubtitleText() const
{
    if (SubtitleWidget)
    {
        return IDialogueSubtitleWidget::Execute_GetSubtitleText(SubtitleWidget.GetObject());
    }
    return "";
}

void UDialogueUIComponent::ShowSubtitle() const
{
    if (SubtitleWidget)
    {
        IDialogueSubtitleWidget::Execute_ShowSubtitle(SubtitleWidget.GetObject());
    }
}

void UDialogueUIComponent::HideSubtitle() const
{
    if (SubtitleWidget)
    {
        IDialogueSubtitleWidget::Execute_HideSubtitle(SubtitleWidget.GetObject());
    }
}

bool operator==(const FSubtitleParam& Lhs,
                const FSubtitleParam& RHS)
{
    return Lhs.SpeakerName == RHS.SpeakerName
           && Lhs.DialogueLine == RHS.DialogueLine
           && Lhs.Type == RHS.Type;
}

bool operator!=(const FSubtitleParam& Lhs,
                const FSubtitleParam& RHS)
{
    return !(Lhs == RHS);
}
