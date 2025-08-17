// Fill out your copyright notice in the Description page of Project Settings.

#include "Graph/Slate/PropertyEditor/DialogueLineNodeDetail.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailGroup.h"
#include "Graph/DialogueGraph.h"
#include "Graph/DialogueGraphAssetFactory.h"
#include "Graph/Node/DialogueEdGraphDialogueLineNode.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"

#define LOCTEXT_NAMESPACE "DialogueLineDetails"

#pragma region 생성자/소멸자

TSharedRef<IDetailCustomization> FDialogueLineNodeDetail::MakeInstance()
{
    return MakeShareable(new FDialogueLineNodeDetail);
}

FDialogueLineNodeDetail::~FDialogueLineNodeDetail()
{
}

#pragma endregion

#pragma region 콜백

void FDialogueLineNodeDetail::OnDialogueLinePropertyChanged(TSharedPtr<IPropertyHandle> PropertyHandle)
{
    if ( PropertyHandle->IsValidHandle() )
    {
        DialogueLineTextContent.Empty(DialogueLineTextContent.Len());
        PropertyHandle->GetValueAsDisplayString(DialogueLineTextContent);
    }
}

#pragma endregion

#pragma region 위젯
void FDialogueLineNodeDetail::CreateDialogueLineTextBox(IDetailLayoutBuilder& DetailBuilder)
{
    IDetailCategoryBuilder& DialogueCategory = DetailBuilder.EditCategory("DialogueLine", FText::FromString(TEXT("대사")));
    // 대사에 대한 텍스트 박스 생성 ( FString을 위한 박스는 너무 작음 )
    if ( const TSharedPtr<IPropertyHandle> DialogueLineHandle = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UDialogueEdGraphDialogueLineNode, DialogueLine));
        DialogueLineHandle.IsUnique() )
    {
        // 기존 대사 속성을 숨긴다.(대신 TextBox를 사용할 예정이므로)
        DetailBuilder.HideProperty(DialogueLineHandle);

        // 프롬프트 속성이 업데이트 될 경우 이 뷰의 프롬프트 값도 업데이트
        DialogueLineHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this,
                                                                                &FDialogueLineNodeDetail::OnDialogueLinePropertyChanged,
                                                                                DialogueLineHandle));

        // TextBox가 배치될 행을 생성
        FDetailWidgetRow& DialogueLineTextRow = DialogueCategory.AddCustomRow(FText::FromString("DialogueLine"));


        // 텍스트 박스의 초기값
        DialogueLineHandle->GetValueAsFormattedString(DialogueLineTextContent);

        // 현재 대사
        DialogueLineTextRow.WholeRowContent()
        [
            SNew(SMultiLineEditableTextBox)
                                           .Margin(FMargin{0, 0, 0, 0})
                                           .HintText(LOCTEXT("WriteDialogueLineHere", "여기에 대사를 작성하세요"))
                                           // .Text(FText::FromString(DialogueLineTextContent))               // 텍스트 내용
                                           .Text(MakeAttributeLambda([DialogueLineHandle]()
                                           {
                                               FString DialogueLineTextContent;
                                               DialogueLineHandle->GetValueAsDisplayString(DialogueLineTextContent);
                                               return FText::FromString(DialogueLineTextContent);
                                           }))
                                           .OnTextChanged_Lambda([DialogueLineHandle](const FText& InText) // 입력 처리
                                           {
                                               if ( DialogueLineHandle.IsValid() && DialogueLineHandle->IsValidHandle() )
                                               {
                                                   // 적절한 업데이트 방법이 아닌듯함
                                                   DialogueLineHandle->SetValue(InText.ToString());
                                               }
                                           })
        ];
    }
    // 현재 지시 프롬프트
    if ( const TSharedRef<IPropertyHandle> CurrentPromptHandle
                = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UDialogueEdGraphDialogueLineNode, CurrentPrompt));
        CurrentPromptHandle.IsUnique() )
    {
        // FDialogueLineCreationPromptDetailTypeCustomization::CustomizeChildren
        DialogueCategory.AddProperty(CurrentPromptHandle);
    }
}
#pragma endregion

#pragma region IDetailCustomization_인터페이스


void FDialogueLineNodeDetail::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
    FDialogueNodeDetail::CustomizeDetails(DetailBuilder);
    {
        // 캐릭터 선택 박스
        if ( const auto CharacterAssetHandle = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UDialogueEdGraphDialogueLineNode, DialogueCharacterAsset)); CharacterAssetHandle->IsValidHandle() )
        {
            IDetailCategoryBuilder& CharacterHandle = DetailBuilder.EditCategory("Character", FText::FromString(TEXT("캐릭터")));
            CharacterHandle.AddProperty(CharacterAssetHandle);
        }
    }

    // 현재 대사 텍스트 박스
    {
        IDetailCategoryBuilder& DialogueCategory = DetailBuilder.EditCategory("DialogueLine");
        DialogueCategory.SetDisplayName(FText::FromString(TEXT("대사")));
        CreateDialogueLineTextBox(DetailBuilder);
    }

    // 생성형 AI 카테고리 ( 기본적으로 안보임, 아이콘을 눌러야 보이게 수정 )
    {
        IDetailCategoryBuilder& GenerationCategory = DetailBuilder.EditCategory("GenerativeAI");
        GenerationCategory.SetDisplayName(FText::FromString(TEXT("생성형 AI")));
        GenerationCategory.SetCategoryVisibility(bShowGenerationCategory);

        // 프롬프트 카테고리
        {
            IDetailGroup& PromptGroup = GenerationCategory.AddGroup("Prompt", FText::FromString(TEXT("프롬프트")));

            // 이전에 사용했던 프롬프트
            if ( const TSharedRef<IPropertyHandle> PromptHistoryHandle = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UDialogueEdGraphDialogueLineNode, PromptHistory));
                PromptHistoryHandle.IsUnique() )
            {
                PromptGroup.AddPropertyRow(PromptHistoryHandle);
            }
        }

        // 생성 옵션 카테고리
        {
            IDetailGroup& GenerationOptionGroup = GenerationCategory.AddGroup("GenerationOption", FText::FromString(TEXT("생성 옵션")));
        }
    }

    if ( const auto Description = GetDescription(DetailBuilder); !Description.IsEmpty() )
    {
        UE_LOG(LogTemp, Warning, TEXT("%s"), *Description);
    }
}

#pragma endregion

FString FDialogueLineNodeDetail::GetDescription(IDetailLayoutBuilder& DetailBuilder) const
{
    TArray<TWeakObjectPtr<UObject>> SelectedObjectsList;
    DetailBuilder.GetObjectsBeingCustomized(SelectedObjectsList);

    if ( SelectedObjectsList.Num() >= 1 )
    {
        if ( UDialogueEdGraphDialogueLineNode* DialogueEdGraphDialogueLineNode =
                Cast<UDialogueEdGraphDialogueLineNode>(SelectedObjectsList[0].Get()) )
        {
            if ( const auto Graph = DialogueEdGraphDialogueLineNode->GetGraph(); Graph != nullptr && Graph->GetOuter() )
            {
                if ( UDialogueGraph* DialogueGraphAsset = Cast<UDialogueGraph>(Graph->GetOuter()) )
                {
                    return DialogueGraphAsset->GetDescription();
                }
            }
            UE_LOG(LogTemp, Warning, TEXT("FDialogueEdDialogueLineNodeDetail::CustomizeDetails"));
        }
    }
    return FString();
}

#undef LOCTEXT_NAMESPACE
