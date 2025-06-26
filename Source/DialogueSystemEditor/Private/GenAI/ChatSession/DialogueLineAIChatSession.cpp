#include "GenAI/ChatSession/DialogueLineAIChatSession.h"

#include "GenAI/ChatHistory/DialogueLineChatHistory.h"
#include "OpenAI/ChatSession_Stream_OpenAI.h"
#include "OpenAI/ChatSettings_OpenAI.h"
// AI Manager 구현부
TSharedRef<FDialogueLineAIChatSession> FDialogueLineAIChatSession::CreateInstance()
{
    TSharedRef<FDialogueLineAIChatSession> Instance = MakeShared<FDialogueLineAIChatSession>();

    if ( !Instance->Initialize() )
    {
        UE_LOG(LogTemp, Error, TEXT("FDialogueLineAIManager 초기화 실패"));
    }

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
