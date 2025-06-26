// Fill out your copyright notice in the Description page of Project Settings.

#include "Graph/Slate/PropertyEditor/DialogueCharacterDetail.h"

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "Graph/Slate/PropertyEditor/DialogueCharacterDetailViewModel.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"


TSharedRef<IPropertyTypeCustomization> FDialogueCharacterDetail::MakeInstance()
{
    return MakeShared<FDialogueCharacterDetail>();
}

#pragma region 자식위젯

void FDialogueCharacterDetail::CreateRootWidget(const TSharedRef<IPropertyHandle>& PropertyHandle,
                                                IDetailChildrenBuilder& ChildBuilder)
{
    const FString SearchString = "Character";

    // 자식 위젯 초기화
    // 기존 위젯도 사용
    // const auto NameWidget  = PropertyHandle->CreatePropertyNameWidget();
    const auto ValueWidget = PropertyHandle->CreatePropertyValueWidget();

    // 여기서만 쓰이는 위젯
    InitializeReadOnlyWidget();
    CreateCharacterAssetButton();

    ChildBuilder
            .AddCustomRow(FText::FromString(SearchString))
            .NameContent()
            [
                // NameWidget
                SNew(STextBlock)
                .Text(FText::FromString(TEXT("캐릭터")))
            ]
            .ValueContent()
            [
                ValueWidget
            ];

    if ( OpenCharacterAssetButton.IsValid() )
    {
        ChildBuilder.AddCustomRow(FText::FromString(SearchString))
                    .WholeRowContent()
        [
            OpenCharacterAssetButton.ToSharedRef()
        ];
    }

    if ( NameTextBox.IsValid() )
    {
        ChildBuilder
                .AddCustomRow(FText::FromString(SearchString))
                .NameContent()
                [
                    SNew(STextBlock)
                    .Text(FText::FromString(TEXT("캐릭터 이름")))
                ]
                .ValueContent()
                .HAlign(HAlign_Fill)
                .VAlign(VAlign_Fill)
                [
                    NameTextBox.ToSharedRef()
                ];
    }


    if ( DescriptionTextBox.IsValid() )
    {
        ChildBuilder
                .AddCustomRow(FText::FromString(SearchString))
                .NameContent()
                [
                    SNew(STextBlock)
                    .Text(FText::FromString(TEXT("설명")))
                ]
                .ValueContent()
                .HAlign(HAlign_Fill)
                .VAlign(VAlign_Fill)
                [
                    DescriptionTextBox.ToSharedRef()
                ];
    }
}

void FDialogueCharacterDetail::CreateCharacterAssetButton()
{
    TWeakPtr<FDialogueCharacterDetail> WeakThisPtr = SharedThis(this);
    SAssignNew(OpenCharacterAssetButton, SButton)
    .VAlign(VAlign_Center)
    .HAlign(HAlign_Center)
    .Text(FText::FromString(TEXT("에셋 열기")))
    .OnClicked_Lambda([WeakThisPtr]
    {
        if ( const TSharedPtr<FDialogueCharacterDetail> ThisPtr = WeakThisPtr.Pin();
            ThisPtr->ViewModel.IsValid() )
        {
            ThisPtr->ViewModel->OpenCharacterAsset();
        }
        return FReply::Handled();
    });
}

void FDialogueCharacterDetail::InitializeReadOnlyWidget()
{
    SAssignNew(NameTextBox, SMultiLineEditableTextBox)
    .IsReadOnly(MakeAttributeLambda([]() { return true; }))
    .Text(MakeAttributeLambda([this]
    {
        return ViewModel.IsValid() ? FText::FromString(ViewModel->GetCharacterName()) : FText::FromString("");
    }));

    SAssignNew(DescriptionTextBox, SMultiLineEditableTextBox)
    .IsReadOnly(MakeAttributeLambda([]() { return true; }))
    .Text(MakeAttributeLambda([this]
    {
        return ViewModel.IsValid() ? FText::FromString(ViewModel->GetDescription()) : FText::FromString("");
    }));

    // ViewModel의 콜백에 등록합니다.
    if ( ViewModel.IsValid() )
    {
        TWeakPtr<FDialogueCharacterDetail> Weak = SharedThis(this);

        if ( NameTextBox.IsValid() )
        {
            ViewModel->SetOnCharacterNameChanged(FSimpleDelegate::CreateLambda([Weak]()
            {
                if ( const TSharedPtr<FDialogueCharacterDetail> This = Weak.Pin();
                    This.IsValid() &&
                    This->ViewModel.IsValid() )
                {
                    This->NameTextBox->SetText(FText::FromString(This->ViewModel->GetCharacterName()));
                }
            }));
        }

        if ( DescriptionTextBox.IsValid() )
        {
            ViewModel->SetOnDescriptionChanged(FSimpleDelegate::CreateLambda([Weak]()
            {
                if ( const TSharedPtr<FDialogueCharacterDetail> This = Weak.Pin();
                    This.IsValid() &&
                    This->ViewModel.IsValid() )
                {
                    This->DescriptionTextBox->SetText(FText::FromString(This->ViewModel->GetDescription()));
                }
            }));
        }
    }
}

#pragma endregion

#pragma region IPropertyTypeCustomization

void FDialogueCharacterDetail::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle,
                                               FDetailWidgetRow& HeaderRow,
                                               IPropertyTypeCustomizationUtils& CustomizationUtils)
{
}

void FDialogueCharacterDetail::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
                                                 IDetailChildrenBuilder& ChildBuilder,
                                                 IPropertyTypeCustomizationUtils& CustomizationUtils)
{
    ViewModel = MakeShared<FDialogueCharacterDetailViewModel>(PropertyHandle);
    CreateRootWidget(PropertyHandle, ChildBuilder);
}

#pragma endregion
