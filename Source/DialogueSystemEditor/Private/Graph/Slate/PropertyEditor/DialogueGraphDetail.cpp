// Fill out your copyright notice in the Description page of Project Settings.

// ReSharper disable CppMemberFunctionMayBeStatic

#include "Graph/Slate/PropertyEditor/DialogueGraphDetail.h"

#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Graph/DialogueGraph.h"
#include "Graph/Slate/PropertyEditor/DialogueGraphDetailViewModel.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"

#define LOCTEXT_NAMESPACE "DialogueGraphDetails"

#pragma region 생성자/소멸자

TSharedRef<IDetailCustomization> FDialogueGraphDetail::MakeInstance()
{
    return MakeShared<FDialogueGraphDetail>();
}

#pragma endregion

/// <summary>
/// IDetailCustomization_인터페이스에서 사용할 하위 위젯
/// </summary>
#pragma region 위젯

void FDialogueGraphDetail::CreateRootWidget(IDetailLayoutBuilder& DetailBuilder)
{
    DetailBuilder.EditCategory(FName("asdf"))
                 .AddCustomRow(FText::FromString("asdf"))
    [
        SNew(SVerticalBox)
        + SVerticalBox::Slot()
        [
            SNew(SButton)
            .Text(FText::FromString(TEXT("Blueprint 호출")))
            .OnClicked_Raw(this, &FDialogueGraphDetail::Foo)
        ]
    ];
    // 연결된 MovieScene 에셋을 여는 버튼
    CreateRelatedMovieSceneWidget(DetailBuilder);

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

void FDialogueGraphDetail::CreateRelatedMovieSceneWidget(IDetailLayoutBuilder& DetailBuilder)
{
    DetailBuilder.EditCategory(FName(TEXT("MovieScene")),
                               FText::FromString(TEXT("MovieScene")))
                 .AddCustomRow(FText::FromString("RelatedMovieSceneAsset"))
                 .WholeRowContent()
    [
        SAssignNew(OpenRelatedMovieSceneButton, SButton)
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Fill)
        .Text(FText::FromString(TEXT("연결된 MovieScene열기")))
        .OnClicked_Lambda([]
        {
            return FReply::Handled();
        })
    ];
}

void FDialogueGraphDetail::CreateTheNumberOfNodeWidget(IDetailLayoutBuilder& DetailBuilder) const
{
    DetailBuilder.EditCategory(FName(TEXT("DialogueGraph")),
                               FText::FromString(TEXT("그래프 정보")))
                 .AddCustomRow(FText::FromString("DialogueGraph Statistics"))
                 .NameContent()
            [
                SNew(STextBlock).Text(FText::FromString(TEXT("전체 노드 수")))
            ]
            .ValueContent()
            [
                SNew(STextBlock).Text(FText::Format(LOCTEXT("NumberOfLineNode", "{0}개"), ViewModel ? ViewModel->CountNode() : 0))
            ];
}

void FDialogueGraphDetail::CreateTheNumberOfSelectNodeWidget(IDetailLayoutBuilder& DetailBuilder) const
{
    DetailBuilder.EditCategory(FName(TEXT("DialogueGraph")),
                               FText::FromString(TEXT("그래프 정보")))
                 .AddCustomRow(FText::FromString("DialogueGraph Statistics"))
                 .NameContent()
            [
                SNew(STextBlock).Text(FText::FromString(TEXT("선택 수")))
            ]
            .ValueContent()
            [
                SNew(STextBlock).Text(FText::Format(LOCTEXT("NumberOfLineNode", "{0}개"), ViewModel ? ViewModel->CountSelectNode() : 0))
            ];
}

void FDialogueGraphDetail::CreateTheNumberOfLineNodeWidget(IDetailLayoutBuilder& DetailBuilder) const
{
    DetailBuilder.EditCategory(FName(TEXT("DialogueGraph")),
                               FText::FromString(TEXT("그래프 정보")))
                 .AddCustomRow(FText::FromString("DialogueGraph Statistics"))
                 .NameContent()
            [
                SNew(STextBlock).Text(FText::FromString(TEXT("대사 노드 수")))
            ]
            .ValueContent()
            [
                SNew(STextBlock).Text(FText::Format(LOCTEXT("NumberOfLineNode", "{0}개"), ViewModel ? ViewModel->CountLineNode() : 0))
            ];
}

void FDialogueGraphDetail::CreateDescriptionTextBoxWidget(IDetailLayoutBuilder& DetailBuilder)
{
    DetailBuilder.EditCategory("Description",
                               FText::FromString(TEXT("상황에 대한 설명(AI가 참고합니다)")))
                 .AddCustomRow(FText::FromString("Description"))
                 .WholeRowContent()
    [
        SAssignNew(DescriptionTextBox, SMultiLineEditableTextBox)
        .Text(MakeAttributeLambda([this]()
        {
            return ViewModel.IsValid() ? FText::FromString(ViewModel->GetDescription()) : FText();
        }))
        .OnTextChanged_Lambda([this](const FText& NewText)
        {
            if ( ViewModel.IsValid() )
            {
                ViewModel->SetDescription(NewText.ToString());
            }
        })
    ];

    if ( ViewModel.IsValid() )
    {
        TWeakPtr<FDialogueGraphDetail> Weak = SharedThis(this);
        ViewModel->SetOnDescriptionChanged(FSimpleDelegate::CreateLambda([Weak]()
        {
            if ( const auto SharedThis = Weak.Pin();
                SharedThis.IsValid() &&
                SharedThis->DescriptionTextBox.IsValid() )
            {
                const FText Text = FText::FromString(SharedThis->ViewModel->GetDescription());
                SharedThis->DescriptionTextBox->SetText(Text);
            }
        }));
    }
}

void FDialogueGraphDetail::CreateMemoTextBoxWidget(IDetailLayoutBuilder& DetailBuilder)
{
    DetailBuilder.EditCategory("Memo",
                               FText::FromString(TEXT("메모(AI가 무시합니다)")))
                 .AddCustomRow(FText::FromString("Memo"))
                 .WholeRowContent()
    [
        SAssignNew(MemoTextBox, SMultiLineEditableTextBox)
        .Text(MakeAttributeLambda([this]()
        {
            return ViewModel.IsValid() ? FText::FromString(ViewModel->GetMemo()) : FText();
        }))
        .OnTextChanged_Lambda([this](const FText& NewText)
        {
            if ( ViewModel.IsValid() )
            {
                ViewModel->SetMemo(NewText.ToString());
            }
        })
    ];

    if ( ViewModel.IsValid() )
    {
        TWeakPtr<FDialogueGraphDetail> Weak = SharedThis(this);
        ViewModel->SetOnDescriptionChanged(FSimpleDelegate::CreateLambda([Weak]()
        {
            if ( const auto SharedThis = Weak.Pin();
                SharedThis.IsValid() &&
                SharedThis->MemoTextBox.IsValid() )
            {
                const FText Text = FText::FromString(SharedThis->ViewModel->GetMemo());
                SharedThis->MemoTextBox->SetText(Text);
            }
        }));
    }
}

#pragma endregion

/// <summary>
/// IDetailCustomization의 인터페이스
/// </summary>
#pragma region IDetailCustomization_인터페이스

void FDialogueGraphDetail::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
    const auto DescriptionHandle = DetailBuilder.GetProperty("Description");
    const auto ParentHandle      = DescriptionHandle->GetParentHandle();

    ViewModel = MakeShared<FDialogueGraphDetailViewModel>(ParentHandle);
    ViewModel->HideProperties(DetailBuilder);

    CreateRootWidget(DetailBuilder);
}

FReply FDialogueGraphDetail::Foo() const
{
    if ( const auto Asset = ViewModel->GetAsset() )
    {
        if ( const auto BPInstance = Asset->GetBlueprintInstance(); BPInstance->IsValidLowLevel() )
        {
            FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(BPInstance);
            UE_LOG(LogTemp, Log, TEXT(""));
        }
    }
    return FReply::Handled();
}

#pragma endregion

#undef LOCTEXT_NAMESPACE
