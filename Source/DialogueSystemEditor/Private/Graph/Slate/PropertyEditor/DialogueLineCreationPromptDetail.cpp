// Fill out your copyright notice in the Description page of Project Settings.

// Resharper의 검사를 생략하기 위한 주석
// ReSharper disable CppPassValueParameterByConstReference

#include "Graph/Slate/PropertyEditor/DialogueLineCreationPromptDetail.h"

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "Editor/Transactor.h"
#include "GenAI/ChatHistory/DialogueLineChatHistory.h"
#include "Graph/DialogueEdGraphNodes.h"
#include "Graph/DialogueGraphAsset.h"
#include "OpenAI/ChatSession_Stream_OpenAI.h"
#include "OpenAI/Type/Item_OpenAI.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"

#define LOCTEXT_NAMESPACE "DialogueLineDetails"

auto ExampleSystemPrompt = TEXT(R"(
캐릭터의 말투를 유지하고 게임 세계와 일치하도록 대화를 작성하십시오.

# 배경

게임 시나리오 작가로서 다음 정보를 제공합니다:
- 캐릭터의 생애, 성장 배경, 성격을 포함한 게임 캐릭터의 배경.
- 캐릭터의 이전 대화 및 말투.
- 게임 세계에 대한 정보.
- 생성해야 할 대화의 설명.
- 캐릭터가 말할 때 사용하는 비언어적 및 언어적 표현.

# 작업

- 제공된 정보를 반영하여 대화를 작성하십시오.
- 캐릭터의 말투가 프롬프트에 제공된 것과 일관성이 있도록 하십시오.
- 게임 세계에 대한 설명을 분석하여 대화를 세계와의 일관성을 유지하면서 게임 세계에 맞추십시오.

# 출력 형식

- 응답은 캐릭터의 대사를 담고 제공된 게임 세계 컨텍스트에 매끄럽게 맞춰지는 문단 또는 여러 문단으로 작성되어야 합니다.)");

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

#pragma endregion

/// <summary>
/// 대화 라인 생성을 위한 AI 채팅 세션 클래스
/// </summary>
#pragma region AI_Session_Class

/// <summary>
/// 대화 라인 생성을 위한 AI 매니저 클래스
/// FOpenAIChatSession_Stream을 컴포지션으로 관리하며 대화 생성 기능을 제공합니다.
/// DialogueLineCreationPromptDetail안에서만 쓰입니다.
/// </summary>
class FDialogueLineAIChatSession
{
public:
    /// <summary>
    /// AI 매니저 인스턴스를 생성합니다.
    /// </summary>
    /// <returns>생성된 AI 매니저의 공유 포인터</returns>
    static TSharedRef<FDialogueLineAIChatSession> CreateInstance();

    /// <summary>
    /// 소멸자
    /// </summary>
    ~FDialogueLineAIChatSession() = default;

    /// <summary>
    /// 채팅 세션에서 메시지를 전송합니다.
    /// </summary>
    /// <param name="Message">전송할 메시지</param>
    void SendMessage(const FString& Message) const;

    /// <summary>
    /// 텍스트 스트리밍 델리게이트를 반환합니다.
    /// </summary>
    /// <returns>텍스트 스트리밍 델리게이트</returns>
    FOnTextStreamed& GetOnTextStreamedDelegate() const;

    /// <summary>
    /// 요청 실패 델리게이트를 반환합니다.
    /// </summary>
    /// <returns>요청 실패 델리게이트</returns>
    FOnRequestFailedDelegate& GetOnRequestFailedDelegate() const;

    /// <summary>
    /// 요청 완료 델리게이트를 반환합니다.
    /// </summary>
    /// <returns>요청 완료 델리게이트</returns>
    FOnRequestItemReceived& GetOnRequestCompletedDelegate() const;

    void SetBackgroundItems(const TArray<FString>& Backgrounds) const;

    /// <summary>
    /// 시스템 프롬프트를 설정합니다.
    /// </summary>
    /// <param name="InSystemPrompt">설정할 시스템 프롬프트</param>
    void SetSystemPrompt(const FString& InSystemPrompt) const;

    /// <summary>
    /// 감정 프롬프트를 설정합니다.
    /// </summary>
    /// <param name="InEmotionPrompt">설정할 감정 프롬프트</param>
    void SetEmotionPrompt(const FString& InEmotionPrompt) const;

    /// <summary>
    /// 설명 프롬프트를 설정합니다.
    /// </summary>
    /// <param name="InDialogueDescriptionPrompt">설정할 설명 프롬프트</param>
    void SetDialogueDescriptionPrompt(const FString& InDialogueDescriptionPrompt) const;

    /// <summary>
    /// 대화라인 설명 프롬프트를 설정합니다.
    /// </summary>
    /// <param name="InLineDescriptionPrompt">설정할 대화라인 설명 프롬프트</param>
    void SetLineDescriptionPrompt(const FString& InLineDescriptionPrompt) const;

    /// <summary>
    /// 채팅 세션이 유효한지 확인합니다.
    /// </summary>
    /// <returns>채팅 세션이 유효하면 true, 그렇지 않으면 false</returns>
    bool IsValid() const;

    /// <summary>
    /// 생성자 (private - CreateInstance를 통해서만 생성 가능)
    /// </summary>
    FDialogueLineAIChatSession();

private:
    /// <summary>
    /// AI 매니저를 초기화합니다.
    /// </summary>
    /// <returns>초기화 성공 여부</returns>
    bool Initialize();

    /// <summary>
    /// 구조화된 출력 선언을 생성합니다.
    /// </summary>
    /// <returns>구조화된 출력 선언의 공유 포인터</returns>
    static TSharedPtr<FStructuredOutputDeclaration> CreateStructuredOutputDeclaration();

private:
    /// <summary>
    /// OpenAI 채팅 세션 (컴포지션)
    /// </summary>
    TSharedPtr<FOpenAIChatSession_Stream> ChatSession;

    /// <summary>
    /// 채팅 히스토리
    /// </summary>
    TSharedPtr<FDialogueLineChatHistory> ChatHistory;
    FString SystemPrompt;
};


// AI Manager 구현부
TSharedRef<FDialogueLineAIChatSession> FDialogueLineAIChatSession::CreateInstance()
{
    TSharedRef<FDialogueLineAIChatSession> Instance = MakeShared<FDialogueLineAIChatSession>();

    if ( !Instance->Initialize() )
    {
        UE_LOG(LogTemp, Error, TEXT("FDialogueLineAIManager 초기화 실패"));
    }

    Instance->SystemPrompt = ExampleSystemPrompt;

    return Instance;
}

FDialogueLineAIChatSession::FDialogueLineAIChatSession()
{
    // 생성자에서는 기본 초기화만 수행
}

bool FDialogueLineAIChatSession::Initialize()
{
    const FString APIKey = FPlatformMisc::GetEnvironmentVariable(TEXT("OPENAI_API_KEY"));

    if ( APIKey.IsEmpty() )
    {
        UE_LOG(LogTemp, Error, TEXT("OPENAI_API_KEY 환경 변수가 설정되지 않았습니다."));
        return false;
    }

    // 채팅 설정 생성
    FOpenAIChatSettings ChatSettings{
        "https://api.openai.com/v1/responses",
        APIKey,
        true,
        EContentType::Text_Stream,
        EOpenAIModel::GPT_4_1_MINI
    };

    // 채팅 히스토리 생성
    ChatHistory = MakeShared<FDialogueLineChatHistory>();

    // 채팅 세션 생성
    ChatSession = FOpenAIChatSession_Stream::CreateChatSession(ChatSettings, ChatHistory);


    if ( !ChatSession.IsValid() )
    {
        UE_LOG(LogTemp, Error, TEXT("OpenAI 채팅 세션 생성 실패"));
        return false;
    }

    // 구조화된 출력 선언 설정
    ChatSession->StructuredOutputDeclaration = CreateStructuredOutputDeclaration();

    return true;
}

TSharedPtr<FStructuredOutputDeclaration> FDialogueLineAIChatSession::CreateStructuredOutputDeclaration()
{
    return MakeShared<FDialogueLineChatStructureDeclaration>();
}

void FDialogueLineAIChatSession::SendMessage(const FString& Message) const
{
    if ( ChatSession.IsValid() )
    {
        ChatSession->SendChat(Message);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("채팅 세션이 유효하지 않습니다."));
    }
}

FOnTextStreamed& FDialogueLineAIChatSession::GetOnTextStreamedDelegate() const
{
    check(ChatSession.IsValid());
    return ChatSession->GetOnPartOfTextStreamedDelegate();
}

FOnRequestFailedDelegate& FDialogueLineAIChatSession::GetOnRequestFailedDelegate() const
{
    check(ChatSession.IsValid());
    return ChatSession->GetOnRequestFailedDelegate();
}

FOnRequestItemReceived& FDialogueLineAIChatSession::GetOnRequestCompletedDelegate() const
{
    return ChatSession->GetOnRequestItemReceived();
}

void FDialogueLineAIChatSession::SetBackgroundItems(const TArray<FString>& Backgrounds) const
{
    if ( ChatSession.IsValid() )
    {
        TArray<FItem_OpenAI> BackgroundItems;
        for ( const FString& Background : Backgrounds )
        {
            BackgroundItems.Emplace(FItem_OpenAI(FInputMessage_OpenAI::CreateSystemTextMessage(FInputTextContent_OpenAI{Background})));
        }
        ChatHistory->SetBackgroundItems(BackgroundItems);
    }
}

void FDialogueLineAIChatSession::SetSystemPrompt(const FString& InSystemPrompt) const
{
    if ( ChatHistory.IsValid() )
    {
        ChatHistory->SetSystemPrompt(InSystemPrompt);
    }
}

void FDialogueLineAIChatSession::SetEmotionPrompt(const FString& InEmotionPrompt) const
{
    if ( ChatHistory.IsValid() )
    {
        ChatHistory->SetEmotionDescriptionItem(InEmotionPrompt);
    }
}

void FDialogueLineAIChatSession::SetDialogueDescriptionPrompt(const FString& InDialogueDescriptionPrompt) const
{
    if ( ChatHistory.IsValid() )
    {
        ChatHistory->SetDialogueDescriptionItem(InDialogueDescriptionPrompt);
    }
}

void FDialogueLineAIChatSession::SetLineDescriptionPrompt(const FString& InLineDescriptionPrompt) const
{
    if ( ChatHistory.IsValid() )
    {
        ChatHistory->SetLineDescriptionItem(InLineDescriptionPrompt);
    }
}

bool FDialogueLineAIChatSession::IsValid() const
{
    return ChatSession.IsValid();
}

#pragma endregion

/// <summary>
/// 생성자 / 소멸자
/// </summary>
#pragma region 생성자/소멸자

TSharedRef<IPropertyTypeCustomization> FDialogueLineCreationPromptDetailTypeCustomization::MakeInstance()
{
    auto Instance = MakeShared<FDialogueLineCreationPromptDetailTypeCustomization>();

    // AI 매니저 인스턴스 생성
    Instance->ChatSession = FDialogueLineAIChatSession::CreateInstance();

    return Instance;
}

#pragma endregion

/// <summary>
/// 콜백들
/// - 프로퍼티 핸들이 관리하는 값이 변경된 경우
/// - UI 이벤트 처리
/// - AI 응답 처리
/// </summary>
#pragma region 콜백

void FDialogueLineCreationPromptDetailTypeCustomization::OnDescriptionChanged(TSharedPtr<IPropertyHandle> DescriptionHandle)
{
    if ( DescriptionHandle.IsValid() && DescriptionHandle->IsValidHandle() )
    {
        LineDescription.Reset(LineDescription.Len());
        DescriptionHandle->GetValueAsDisplayString(LineDescription);
    }
}

void FDialogueLineCreationPromptDetailTypeCustomization::OnEmotionChanged(TSharedPtr<IPropertyHandle> EmotionHandle)
{
    if ( EmotionHandle.IsValid() && EmotionHandle->IsValidHandle() )
    {
        Emotion.Reset(Emotion.Len());
        EmotionHandle->GetValueAsDisplayString(Emotion);
    }
}

/// <summary>
/// 사용자가 Enter 키를 눌렀을 때 처리하는 콜백
/// </summary>
/// <param name="InKeyEvent">키 이벤트</param>
/// <returns>이벤트 처리 결과</returns>
FReply FDialogueLineCreationPromptDetailTypeCustomization::HandleEnterKeyPress(const FKeyEvent& InKeyEvent)
{
    if ( InKeyEvent.GetKey() == EKeys::Enter && !InKeyEvent.IsControlDown() )
    {
        if ( ChatSession.IsValid() )
        {
            SendMessage();
        }

        // 기존 프롬프트 입력창을 비운다.
        UserInstruction = ""; // TODO: 만약 Cancel이면 이 라인을 제거하고, 다시 SetText 
        UserInstructionTextBox->SetText(FText::FromString(UserInstruction));

        // 기존의 결과창을 비운다.
        AITextResponseTextBox->SetText(FText::FromString(""));

        return FReply::Handled();
    }
    return FReply::Unhandled();
}

/// <summary>
/// 사용자 지시사항 텍스트가 변경되었을 때 처리하는 콜백
/// </summary>
/// <param name="InText">변경된 텍스트</param>
void FDialogueLineCreationPromptDetailTypeCustomization::OnUserInstructionTextChanged(const FText& InText)
{
    UserInstruction = InText.ToString();
}

/// <summary>
/// AI에서 텍스트가 스트리밍되었을 때 처리하는 콜백
/// </summary>
/// <param name="ItemId">아이템 ID</param>
/// <param name="Role">역할</param>
/// <param name="StreamedText">스트리밍된 텍스트</param>
void FDialogueLineCreationPromptDetailTypeCustomization::HandleTextStreamingResponse(const FString ItemId,
                                                                                     const FString Role,
                                                                                     const FString StreamedText) const
{
    AsyncTask(ENamedThreads::GameThread, [this, ItemId, Role, StreamedText]
    {
        const FString CurrentText = AITextResponseTextBox->GetText().ToString();
        const FText NewText       = FText::Format(LOCTEXT("ChatMessage", "{0}{1}"), FText::FromString(CurrentText), FText::FromString(StreamedText));
        AITextResponseTextBox->SetText(NewText);
    });
}

/// <summary>
/// AI 요청이 실패했을 때 처리하는 콜백
/// </summary>
/// <param name="Reason">실패 이유</param>
/// <param name="DetailMessage">상세 메시지</param>
void FDialogueLineCreationPromptDetailTypeCustomization::HandleRequestFailure(ERequestFailedReason Reason,
                                                                              const FString DetailMessage) const
{
    AsyncTask(ENamedThreads::GameThread, [this, Reason, DetailMessage]
    {
        switch ( Reason )
        {
        case ERequestFailedReason::InvalidInputFormat:
            FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("Error_InvalidInputFormat", "Invalid Input Format.\n{0}"), FText::FromString(DetailMessage)));
            break;
        case ERequestFailedReason::ConnectionError:
            FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("Error_ConnectionError", "Connection Error.\n{0}"), FText::FromString(DetailMessage)));
            break;
        case ERequestFailedReason::Cancelled:
            FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("Error_Cancelled", "Cancelled.\n{0}"), FText::FromString(DetailMessage)));
            break;
        case ERequestFailedReason::Timeout:
            FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("Error_Timeout", "Timeout.\n{0}"), FText::FromString(DetailMessage)));
            break;
        case ERequestFailedReason::Etc:
            FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("Error_Unknown", "Unknown Error.\n{0}"), FText::FromString(DetailMessage)));
            break;
        }
    });
}

/// <summary>
/// AI 요청이 완료되었을 때 처리하는 콜백
/// </summary>
/// <param name="OutputItem">출력 아이템</param>
/// <param name="PropertyHandle">대화라인 프로퍼티 핸들</param>
void FDialogueLineCreationPromptDetailTypeCustomization::HandleRequestCompletion(FOutputItem_OpenAI OutputItem,
                                                                                 TSharedPtr<IPropertyHandle> PropertyHandle) const
{
    AsyncTask(ENamedThreads::GameThread, [this, PropertyHandle, OutputItem]
    {
        if ( OutputItem.Message.IsSet() &&
             OutputItem.Message.GetValue().Content.Num() > 0 &&
             OutputItem.Message.GetValue().Content[0].TextContent.IsSet()
        )
        {
            const FOutputTextContent_OpenAI Text = OutputItem.Message.GetValue().Content[0].TextContent.GetValue();
            auto Json                            = ToJson(&Text.Text);
            if ( Json->HasTypedField(TEXT("DialogueLine"), EJson::String) )
            {
                const FScopedTransaction Transaction(LOCTEXT("ApplyChatResult", "AI의 답변을 대사에 적용"));

                const FString DialogueLine = Json->GetStringField(TEXT("DialogueLine"));
                PropertyHandle->SetValueFromFormattedString(DialogueLine);
            }
        }
    });
}

#pragma endregion

/// <summary>
/// IPropertyTypeCustomization 안에서 사용할 위젯 모음
/// </summary>
#pragma region 위젯

void FDialogueLineCreationPromptDetailTypeCustomization::CreateOutputTextPanel(const TSharedPtr<SVerticalBox>& DescriptionVerticalBox)
{
    AITextResponseTextBox = SNew(SMultiLineEditableTextBox)
        .WrapTextAt(WrapTextAt)
        .IsReadOnly(true)
        .AllowContextMenu(true)
        .HintText(LOCTEXT("AIOutput", "생성형 AI의 답변이 여기에 표시됩니다."))
        .Text(FText::FromString(AIOutput)); // 텍스트 내용

    DescriptionVerticalBox->AddSlot()
                          .AutoHeight()
    [
        SNew(STextBlock)
        .Text(LOCTEXT("AIOutputLabel", "AI의 답변"))
    ];
    DescriptionVerticalBox->AddSlot()
                          .AutoHeight()
    [
        AITextResponseTextBox->AsShared()
    ];
}

void FDialogueLineCreationPromptDetailTypeCustomization::CreateEmotionWidget(const TSharedPtr<IPropertyHandle>& EmotionHandle,
                                                                             const TSharedPtr<SVerticalBox>& DescriptionVerticalBox)
{
    Emotion.Reset(Emotion.Len());
    EmotionHandle->GetValueAsFormattedString(Emotion);

    DescriptionVerticalBox->AddSlot()
                          .AutoHeight()
    [
        SNew(STextBlock)
        .Text(LOCTEXT("Emotion", "감정에 대한 설명 (생성 요청 후에도 유지됨)"))
    ];

    DescriptionVerticalBox->AddSlot()
                          .AutoHeight()
    [
        SNew(SMultiLineEditableTextBox)
                                       .WrapTextAt(WrapTextAt)
                                       .HintText(LOCTEXT("WriteEmotionHere", "AI가 이해할 수 있도록 여기에 감정에 대한 설명을 작성하세요"))
                                       .Text(FText::FromString(Emotion))                                // 텍스트 내용
                                       .OnTextChanged_Lambda([this, EmotionHandle](const FText& InText) // 입력 처리
                                       {
                                           if ( EmotionHandle.IsValid() && EmotionHandle->IsValidHandle() )
                                           {
                                               EmotionHandle->SetValue(InText.ToString());
                                           }
                                       })
    ];
}

void FDialogueLineCreationPromptDetailTypeCustomization::CreateDescriptionWidget(const TSharedPtr<IPropertyHandle>& DescriptionHandle,
                                                                                 const TSharedPtr<SVerticalBox>& DescriptionVerticalBox)
{
    LineDescription.Empty(LineDescription.Len());
    DescriptionHandle->GetValueAsFormattedString(LineDescription);

    DescriptionVerticalBox->AddSlot()
                          .AutoHeight()
    [
        SNew(STextBlock)
        .Text(LOCTEXT("Description", "대사에 대한 설명 (생성 요청 후에도 유지됨)"))
    ];

    DescriptionVerticalBox->AddSlot()
                          .AutoHeight()
    [
        SNew(SMultiLineEditableTextBox)
                                       .WrapTextAt(WrapTextAt)
                                       .HintText(LOCTEXT("WriteDialogueLineHere", "AI가 이해할 수 있도록 여기에 상황에 대한 설명을 작성하세요"))
                                       .Text(FText::FromString(LineDescription))                            // 텍스트 내용
                                       .OnTextChanged_Lambda([this, DescriptionHandle](const FText& InText) // 입력 처리
                                       {
                                           if ( DescriptionHandle.IsValid() && DescriptionHandle->IsValidHandle() )
                                           {
                                               DescriptionHandle->SetValue(InText.ToString());
                                           }
                                       })
    ];
}

void FDialogueLineCreationPromptDetailTypeCustomization::CreateUserInstructionWidget(
    const TSharedPtr<SVerticalBox>& DescriptionVerticalBox)
{
    DescriptionVerticalBox->AddSlot()
                          .AutoHeight()
    [
        SNew(STextBlock)
        .Text(LOCTEXT("UserInstruction", "사용자의 요청사항( 전송 후 사라짐 )"))
    ];

    DescriptionVerticalBox->AddSlot()
                          .AutoHeight()
    [
        SNew(SHorizontalBox)
        + SHorizontalBox::Slot()
        .FillWidth(0.9f)
        [
            SAssignNew(UserInstructionTextBox,
                       SMultiLineEditableTextBox)
                                                 .WrapTextAt(WrapTextAt)
                                                 .HintText(LOCTEXT("WriteEmotionHere", "기타 AI가 참고해야할 내용을 여기에 입력하세요 (이 영역은 일반 ChatGPT, Claude의 채팅창에 해당합니다.)"))
                                                 .Text(FText::FromString(UserInstruction)) // 텍스트 내용
                                                 .OnKeyDownHandler_Lambda([this](const FGeometry& _,
                                                                                 const FKeyEvent& InKeyEvent)
                                                 {
                                                     return HandleEnterKeyPress(InKeyEvent);
                                                 })
                                                 .OnTextChanged_Lambda([this](const FText& InText)
                                                 {
                                                     OnUserInstructionTextChanged(InText);
                                                 })

        ]
        + SHorizontalBox::Slot()
        .FillWidth(0.1f)
        [
            SNew(SButton)
            .HAlign(HAlign_Center)
            .VAlign(VAlign_Center)
            .Text(LOCTEXT("Send", "전송"))
            .OnClicked_Lambda([this]() -> FReply
            {
                if ( ChatSession.IsValid() )
                {
                    SendMessage();
                }

                // 기존 프롬프트 입력창을 비운다.
                UserInstruction = ""; // TODO: 만약 Cancel이면 이 라인을 제거하고, 다시 SetText 
                UserInstructionTextBox->SetText(FText::FromString(UserInstruction));

                // 기존의 결과창을 비운다.
                AITextResponseTextBox->SetText(FText::FromString(""));

                return FReply::Handled();
            })
        ]
    ];
}

void FDialogueLineCreationPromptDetailTypeCustomization::CreateDialogueDescriptionWidget(TSharedRef<IPropertyHandle> PropertyHandle,
                                                                                         const TSharedPtr<SVerticalBox>& DescriptionVerticalBox)
{
    TArray<UObject*> OuterObjects;
    PropertyHandle->GetOuterObjects(OuterObjects);
    if ( OuterObjects.Num() > 0 )
    {
        const UObject* OuterObject = OuterObjects[0];
        if ( const UEdGraph* DialogueGraph = Cast<UEdGraph>(OuterObject->GetOuter()) )
        {
            if ( const UDialogueGraphAsset* DialogueGraphAsset = Cast<UDialogueGraphAsset>(DialogueGraph->GetOuter()) )
            {
                const FString GraphDescription = DialogueGraphAsset->GetDescription();

                // 생성형 AI가 참고할 수 있도록 별도의 프롶퍼티로 기록
                DialogueDescription = GraphDescription;

                DescriptionVerticalBox->AddSlot()
                                      .AutoHeight()
                [
                    SNew(STextBlock)
                    .Text(LOCTEXT("DialogueDescription", "이 대화에 대한 설명 ( AI가 참고합니다 )"))
                ];

                DescriptionVerticalBox->AddSlot()
                                      .AutoHeight()
                [
                    SNew(SMultiLineEditableTextBox)
                    .IsReadOnly(true)
                    .Text(FText::FromString(GraphDescription))
                ];
            }
        }
    }
}

void FDialogueLineCreationPromptDetailTypeCustomization::SendMessage() const
{
    if ( !FString(ExampleSystemPrompt).TrimStartAndEnd().IsEmpty() )
    {
        ChatSession->SetSystemPrompt(ExampleSystemPrompt);
    }

    if ( !DialogueDescription.TrimStartAndEnd().IsEmpty() )
    {
        ChatSession->SetDialogueDescriptionPrompt(DialogueDescription);
    }

    if ( !Emotion.TrimStartAndEnd().IsEmpty() )
    {
        ChatSession->SetEmotionPrompt(Emotion);
    }

    if ( !LineDescription.TrimStartAndEnd().IsEmpty() )
    {
        ChatSession->SetLineDescriptionPrompt(LineDescription);
    }

    ChatSession->SendMessage(UserInstruction);
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
    HeaderRow.NameContent()
    [
        SNew(STextBlock)
        .Text(FText::FromString(TEXT("대사 생성 AI")))
    ];
}

void FDialogueLineCreationPromptDetailTypeCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
                                                                           IDetailChildrenBuilder& ChildBuilder,
                                                                           IPropertyTypeCustomizationUtils& CustomizationUtils)
{
    if ( !PropertyHandle->IsValidHandle() )
    {
        return;
    }

    FDetailWidgetRow& PromptRow                           = ChildBuilder.AddCustomRow(FText::FromString("Prompt"));
    const TSharedPtr<SVerticalBox> DescriptionVerticalBox = SNew(SVerticalBox);

    PromptRow
    [
        DescriptionVerticalBox->AsShared()
    ];

    // 현재 대화에 대한 설명
    {
        CreateDialogueDescriptionWidget(PropertyHandle, DescriptionVerticalBox);
    }

    // 상황에 대한 입력
    if ( const TSharedPtr<IPropertyHandle> DescriptionHandle =
                PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FDialogueLineCreationPrompt, Description));
        DescriptionHandle.IsValid() && DescriptionHandle->IsValidHandle() )
    {
        // 프로퍼티가 변경될 경우 호출할 델리게이트 등록
        DescriptionHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this,
                                                                               &FDialogueLineCreationPromptDetailTypeCustomization::OnDescriptionChanged,
                                                                               DescriptionHandle));
        CreateDescriptionWidget(DescriptionHandle, DescriptionVerticalBox);
    }

    // 감정에 대한 입력
    if ( const TSharedPtr<IPropertyHandle> EmotionHandle =
                PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FDialogueLineCreationPrompt, Emotion));
        EmotionHandle.IsValid() && EmotionHandle->IsValidHandle() )
    {
        EmotionHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this,
                                                                           &FDialogueLineCreationPromptDetailTypeCustomization::OnEmotionChanged,
                                                                           EmotionHandle));
        CreateEmotionWidget(EmotionHandle, DescriptionVerticalBox);
    }

    // AI의 답변
    CreateOutputTextPanel(DescriptionVerticalBox);

    // 사용자가 추가적으로 제공하는 힌트    
    CreateUserInstructionWidget(DescriptionVerticalBox);

    // 콜백 등록
    if ( ChatSession.IsValid() && ChatSession->IsValid() )
    {
        // 텍스트가 스트림된 경우
        ChatSession->GetOnTextStreamedDelegate().BindSP(this, &FDialogueLineCreationPromptDetailTypeCustomization::HandleTextStreamingResponse);

        const auto ParentProp = PropertyHandle->GetParentHandle();
        auto Prop             = ParentProp->GetChildHandle(GET_MEMBER_NAME_CHECKED(UDialogueEdGraphDialogueLineNode, DialogueLine));

        // 오류시 콜백
        ChatSession->GetOnRequestFailedDelegate().AddSP(this, &FDialogueLineCreationPromptDetailTypeCustomization::HandleRequestFailure);

        // 요청 완료 콜백
        ChatSession->GetOnRequestCompletedDelegate().AddSPLambda(this, [this, Prop](FOutputItem_OpenAI OutputItem)
        {
            HandleRequestCompletion(OutputItem, Prop);
        });
    }
}
#pragma endregion


#undef LOCTEXT_NAMESPACE
