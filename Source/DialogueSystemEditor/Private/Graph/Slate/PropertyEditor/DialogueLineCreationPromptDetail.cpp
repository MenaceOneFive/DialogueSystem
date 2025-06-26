// Fill out your copyright notice in the Description page of Project Settings.

// Resharper의 검사를 생략하기 위한 주석
// ReSharper disable CppPassValueParameterByConstReference

#include "Graph/Slate/PropertyEditor/DialogueLineCreationPromptDetail.h"

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IDetailGroup.h"
#include "GenAI/ChatSession/DialogueLineAIChatSession.h"
#include "Graph/Node/DialogueEdGraphNode.h"
#include "Graph/Slate/PropertyEditor/DialogueLineCreationPromptDetailViewModel.h"
#include "Json/JsonUtil.h"
#include "OpenAI/Type/Item_OpenAI.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"

#define LOCTEXT_NAMESPACE "DialogueLineDetails"


/// <summary>
/// 채팅의 결과를 특정 JSON형태로 출력하기 위한 정의
/// </summary>
#pragma region 구조화된_출력

/// <summary>
/// 대화 라인 채팅 구조화 출력 선언 클래스
/// </summary>
class FDialogueLineChatStructureDeclaration final : public FStructuredOutputDeclaration
{
public:
    /// <summary>
    /// 생성자
    /// </summary>
    FDialogueLineChatStructureDeclaration()
    {
        Name        = "DialogueLine";
        Description = TEXT("생성된 대사와 사용자에게 알릴 정보");
    }

    /// <summary>
    /// 구조화된 출력 빌더를 반환합니다.
    /// </summary>
    /// <param name="Context">빌더 컨텍스트</param>
    /// <returns>구조화된 출력 빌더</returns>
    virtual FStructuredOutputBuilder GetOutputDeclarationBuilder(EStructuredOutputBuilderContext Context) const override
    {
        FStructuredOutputBuilder Builder;
        Builder.SetContext(Context)
               .AddRequiredStringProperty("DialogueLine", TEXT("생성된 대사"))
               .AddRequiredStringProperty("SystemMessage", TEXT("대사와 상관없는 AI의 메시지"));
        return Builder;
    }
};

TSharedPtr<FStructuredOutputDeclaration> FDialogueLineAIChatSession::CreateStructuredOutputDeclaration()
{
    return MakeShared<FDialogueLineChatStructureDeclaration>();
}

#pragma endregion

/// <summary>
/// 생성자 / 소멸자
/// </summary>
#pragma region 생성자/소멸자

TSharedRef<IPropertyTypeCustomization> FDialogueLineCreationPromptDetailTypeCustomization::MakeInstance()
{
    return MakeShared<FDialogueLineCreationPromptDetailTypeCustomization>();
}

#pragma endregion

/// <summary>
/// IPropertyTypeCustomization 안에서 사용할 위젯 모음
/// </summary>
#pragma region 위젯

void FDialogueLineCreationPromptDetailTypeCustomization::CreateOutputTextPanel(const TSharedPtr<SVerticalBox>& DescriptionVerticalBox)
{
    if ( ViewModel.IsValid() )
    {
        AITextResponseTextBox = SNew(SMultiLineEditableTextBox)
            .WrapTextAt(WrapTextAt)
            .IsReadOnly(true)
            .AllowContextMenu(true)
            .HintText(LOCTEXT("AIOutput", "생성형 AI의 답변이 여기에 표시됩니다."))
            .Text(MakeAttributeLambda([this]()
            {
                return FText::FromString(ViewModel->GetAIOutput());
            }));
        DescriptionVerticalBox->AddSlot().AutoHeight()
        [
            SNew(STextBlock)
            .Text(LOCTEXT("AIOutputLabel", "AI의 답변"))
        ];
        DescriptionVerticalBox->AddSlot().AutoHeight()
        [
            AITextResponseTextBox->AsShared()
        ];
    }
}

void FDialogueLineCreationPromptDetailTypeCustomization::CreateUserInstructionWidget(const TSharedPtr<SVerticalBox>& DescriptionVerticalBox)
{
    DescriptionVerticalBox->AddSlot().AutoHeight()
    [
        SNew(STextBlock)
        .Text(LOCTEXT("UserInstruction", "사용자의 요청사항( 전송 후 사라짐 )"))
    ];
    DescriptionVerticalBox->AddSlot().AutoHeight()
    [
        SNew(SHorizontalBox)
        + SHorizontalBox::Slot().FillWidth(0.9f)
        [
            SAssignNew(UserInstructionTextBox, SMultiLineEditableTextBox)
            .Text(MakeAttributeLambda([this]()
            {
                return FText::FromString(ViewModel->GetUserInstruction());
            }))
            .OnTextChanged(FOnTextChanged::CreateSPLambda(this, [this](const FText& InText)
            {
                this->ViewModel->OnUserInstructionTextChanged(InText);
            }))
            .OnKeyDownHandler(FOnKeyDown::CreateSPLambda(this, [this](const FGeometry& _,
                                                                      const FKeyEvent& KeyEvent)
            {
                if ( const FReply Res = this->ViewModel->HandleEnterKeyPress(_, KeyEvent); Res.IsEventHandled() )
                {
                    return FReply::Handled();
                }
                return FReply::Unhandled();
            }))
        ]
    ];
}

void FDialogueLineCreationPromptDetailTypeCustomization::CreateEmotionWidget(
    const TSharedPtr<SVerticalBox>& DescriptionVerticalBox) const
{
    DescriptionVerticalBox->AddSlot().AutoHeight()
    [
        SNew(STextBlock)
        .Text(LOCTEXT("Emotion", "감정에 대한 설명 (생성 요청 후에도 유지됨)"))
    ];
    DescriptionVerticalBox->AddSlot().AutoHeight()
    [
        SNew(SMultiLineEditableTextBox)
        .IsReadOnly(false)
        .Text(MakeAttributeLambda([this]()
        {
            return FText::FromString(ViewModel->GetEmotion());
        }))
        .OnTextChanged_Lambda([this](const FText& InText)
        {
            if ( ViewModel.IsValid() )
            {
                ViewModel->SetEmotion(InText.ToString());
            }
        })
    ];
}

void FDialogueLineCreationPromptDetailTypeCustomization::CreateDescriptionWidget(
    const TSharedPtr<SVerticalBox>& DescriptionVerticalBox) const
{
    DescriptionVerticalBox->AddSlot().AutoHeight()
    [
        SNew(STextBlock)
        .Text(LOCTEXT("Description", "대사에 대한 설명 (생성 요청 후에도 유지됨)"))
    ];
    DescriptionVerticalBox->AddSlot().AutoHeight()
    [
        SNew(SMultiLineEditableTextBox)
        .IsReadOnly(false)
        .Text(MakeAttributeLambda([this]()
        {
            return FText::FromString(ViewModel->GetLineDescription());
        }))
        .OnTextChanged_Lambda([this](const FText& InText)
        {
            if ( ViewModel.IsValid() )
            {
                ViewModel->SetLineDescription(InText.ToString());
            }
        })
    ];
}

void FDialogueLineCreationPromptDetailTypeCustomization::CreateDialogueDescriptionWidget(
    const TSharedPtr<IPropertyHandle>& PropertyHandle,
    const TSharedPtr<SVerticalBox>& DescriptionVerticalBox) const
{
    const FString GraphDescription = ViewModel->GetDialogueDescription();

    // 생성형 AI가 참고할 수 있도록 별도의 프로퍼티로 기록
    if ( ViewModel.IsValid() )
    {
        ViewModel->SetDialogueDescription(GraphDescription);
    }

    DescriptionVerticalBox->AddSlot().AutoHeight()
    [
        SNew(STextBlock)
        .Text(LOCTEXT("DialogueDescription", "이 대화에 대한 설명 ( AI가 참고합니다 )"))
    ];

    DescriptionVerticalBox->AddSlot().AutoHeight()
    [
        SNew(SMultiLineEditableTextBox)
        .IsReadOnly(true)
        .Text(FText::FromString(GraphDescription))
    ];
}


#pragma endregion

/// <summary>
/// IPropertyTypeCustomization 인터페이스
/// 프로퍼티 디테일 창에서 사용할 UI를 정의합니다.
/// </summary>
#pragma region IPropertyTypeCustomization_인터페이스

void FDialogueLineCreationPromptDetailTypeCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle,
                                                                         FDetailWidgetRow& HeaderRow,
                                                                         IPropertyTypeCustomizationUtils& CustomizationUtils)
{
}

void FDialogueLineCreationPromptDetailTypeCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
                                                                           IDetailChildrenBuilder& ChildBuilder,
                                                                           IPropertyTypeCustomizationUtils& CustomizationUtils)
{
    if ( !PropertyHandle->IsValidHandle() )
    {
        return;
    }

    // ViewModel 초기화
    ViewModel = MakeShared<FDialogueLineCreationPromptDetailViewModel>(PropertyHandle);
    ViewModel->BindCallbacks();

    IDetailGroup& GenerativeAIGroup = ChildBuilder.AddGroup("GenerativeAI", FText::FromString(TEXT("대사 생성 AI")));

    FDetailWidgetRow& PromptRow                           = GenerativeAIGroup.AddWidgetRow();
    const TSharedPtr<SVerticalBox> DescriptionVerticalBox = SNew(SVerticalBox);

    PromptRow
    [
        DescriptionVerticalBox->AsShared()
    ];

    // 현재 대화에 대한 설명 (그래프 귀속)
    CreateDialogueDescriptionWidget(PropertyHandle, DescriptionVerticalBox);

    // 상황에 대한 입력
    CreateDescriptionWidget(DescriptionVerticalBox);

    // 감정에 대한 입력
    CreateEmotionWidget(DescriptionVerticalBox);

    // AI의 답변
    CreateOutputTextPanel(DescriptionVerticalBox);

    // 사용자가 추가적으로 제공하는 힌트    
    CreateUserInstructionWidget(DescriptionVerticalBox);

    // 콜백 등록
    if ( const TSharedPtr<FDialogueLineAIChatSession> DialogueLineAIChatSession = ViewModel->GetChatSession();
        ViewModel.IsValid() && DialogueLineAIChatSession.IsValid() )
    {
    }
}
#pragma endregion

#undef LOCTEXT_NAMESPACE
