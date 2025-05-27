// Fill out your copyright notice in the Description page of Project Settings.

// ReSharper disable CppMemberFunctionMayBeStatic
#include "Graph/Slate/PropertyEditor/DialogueGraphDetailCustomization.h"

#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Graph/DialogueGraphAsset.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"

#define LOCTEXT_NAMESPACE "DialogueGraphDetails"

#pragma region 생성자/소멸자

FDialogueGraphDetailCustomization::FDialogueGraphDetailCustomization()
{
}

FDialogueGraphDetailCustomization::~FDialogueGraphDetailCustomization()
{
}


TSharedRef<IDetailCustomization> FDialogueGraphDetailCustomization::MakeInstance()
{
    return MakeShared<FDialogueGraphDetailCustomization>();
}

#pragma endregion

/// <summary>
/// IDetailCustomization_인터페이스에서 사용할 하위 위젯
/// </summary>
#pragma region 위젯

void FDialogueGraphDetailCustomization::OpenRelatedMovieSceneWidget(const IDetailLayoutBuilder& DetailBuilder)
{
}

void FDialogueGraphDetailCustomization::CreateTheNumberOfNodeWidget(IDetailLayoutBuilder& DetailBuilder)
{
    const auto DialogueGraphAssets = DetailBuilder.GetSelectedObjects();
    if ( DialogueGraphAssets.IsEmpty() )
    {
        return;
    }

    const auto DialogueGraphAsset = Cast<UDialogueGraphAsset>(DialogueGraphAssets[0]);

    DetailBuilder.EditCategory(FName(TEXT("DialogueGraph")),
                               FText::FromString(TEXT("그래프 정보")))
                 .AddCustomRow(FText::FromString("DialogueGraph Statistics"))
                 .NameContent()
            [
                SNew(STextBlock).Text(FText::FromString(TEXT("전체 노드 수")))
            ]
            .ValueContent()
            [
                SNew(STextBlock).Text(FText::Format(LOCTEXT("NumberOfLineNode", "{0}개"), DialogueGraphAsset->CountNode()))
            ];
}

void FDialogueGraphDetailCustomization::CreateTheNumberOfSelectNodeWidget(IDetailLayoutBuilder& DetailBuilder)
{
    const auto DialogueGraphAssets = DetailBuilder.GetSelectedObjects();
    if ( DialogueGraphAssets.IsEmpty() )
    {
        return;
    }
    const auto DialogueGraphAsset = Cast<UDialogueGraphAsset>(DialogueGraphAssets[0]);

    DetailBuilder.EditCategory(FName(TEXT("DialogueGraph")),
                               FText::FromString(TEXT("그래프 정보")))
                 .AddCustomRow(FText::FromString("DialogueGraph Statistics"))
                 .NameContent()
            [
                SNew(STextBlock).Text(FText::FromString(TEXT("선택 수")))
            ]
            .ValueContent()
            [
                SNew(STextBlock).Text(FText::Format(LOCTEXT("NumberOfLineNode", "{0}개"), DialogueGraphAsset->CountSelectNode()))
            ];
}


void FDialogueGraphDetailCustomization::CreateTheNumberOfLineNodeWidget(IDetailLayoutBuilder& DetailBuilder)
{
    const auto DialogueGraphAssets = DetailBuilder.GetSelectedObjects();
    if ( DialogueGraphAssets.IsEmpty() )
    {
        return;
    }
    const auto DialogueGraphAsset = Cast<UDialogueGraphAsset>(DialogueGraphAssets[0]);

    DetailBuilder.EditCategory(FName(TEXT("DialogueGraph")),
                               FText::FromString(TEXT("그래프 정보")))
                 .AddCustomRow(FText::FromString("DialogueGraph Statistics"))
                 .NameContent()
            [
                SNew(STextBlock).Text(FText::FromString(TEXT("대사 노드 수")))
            ]
            .ValueContent()
            [
                SNew(STextBlock).Text(FText::Format(LOCTEXT("NumberOfLineNode", "{0}개"), DialogueGraphAsset->CountLineNode()))
            ];
}

void FDialogueGraphDetailCustomization::CreateDescriptionTextBoxWidget(IDetailLayoutBuilder& DetailBuilder)
{
    DescriptionHandle = DetailBuilder.GetProperty("Description");

    DetailBuilder.HideProperty(DescriptionHandle);

    DetailBuilder.EditCategory("Description",
                               FText::FromString(TEXT("상황에 대한 설명(AI가 참고합니다)")))
                 .AddCustomRow(FText::FromString("Description"))
                 .WholeRowContent()
    [
        SNew(SMultiLineEditableTextBox)
        .Text(MakeAttributeLambda([this]()
        {
            if ( this && this->DescriptionHandle.IsValid() )
            {
                FString Value;
                this->DescriptionHandle->GetValueAsFormattedString(Value);
                return FText::FromString(Value);
            }
            return FText();
        }))
        .OnTextChanged_Lambda([this](const FText& NewText)
        {
            if ( this && this->DescriptionHandle.IsValid() )
            {
                this->DescriptionHandle->SetValueFromFormattedString(NewText.ToString());
            }
        })
    ];
}

void FDialogueGraphDetailCustomization::CreateMemoTextBoxWidget(IDetailLayoutBuilder& DetailBuilder)
{
    MemoHandle = DetailBuilder.GetProperty("Memo");

    DetailBuilder.HideProperty(MemoHandle);

    DetailBuilder.EditCategory("Memo",
                               FText::FromString(TEXT("메모(AI가 무시합니다)")))
                 .AddCustomRow(FText::FromString("Memo"))
                 .WholeRowContent()
    [
        SNew(SMultiLineEditableTextBox)
        .Text(MakeAttributeLambda([this]()
        {
            if ( this && this->MemoHandle.IsValid() )
            {
                FString Value;
                this->MemoHandle->GetValueAsFormattedString(Value);
                return FText::FromString(Value);
            }
            return FText();
        }))
        .OnTextChanged_Lambda([this](const FText& NewText)
        {
            if ( this && this->MemoHandle.IsValid() )
            {
                this->MemoHandle->SetValueFromFormattedString(NewText.ToString());
            }
        })
    ];
}
#pragma endregion


/// <summary>
/// IDetailCustomization의 인터페이스
/// </summary>
#pragma region IDetailCustomization_인터페이스

void FDialogueGraphDetailCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
    // 연결되어 있는 MovieScene를 여는 버튼 위젯
    OpenRelatedMovieSceneWidget(DetailBuilder);

    // 노드의 개수에 대한 정보들
    // 전체 노드의 개수를 반환하는 위젯
    CreateTheNumberOfNodeWidget(DetailBuilder);

    // 선택 노드의 개수를 반환하는 위젯
    CreateTheNumberOfSelectNodeWidget(DetailBuilder);

    // 대사 노드의 개수를 반환하는 위젯
    CreateTheNumberOfLineNodeWidget(DetailBuilder);

    // 해당 장면에 대한 기본적인 설명
    CreateDescriptionTextBoxWidget(DetailBuilder);

    // 편집자가 남기고 싶은 메모
    CreateMemoTextBoxWidget(DetailBuilder);
}

#pragma endregion

#undef LOCTEXT_NAMESPACE
