#include "GenAI/ChatHistory/DialogueLineChatHistory.h"
#include "OpenAI/Type/InputMessage_OpenAI.h"
#include "OpenAI/Type/InputTextContent_OpenAI.h"
#include "OpenAI/Type/Item_OpenAI.h"

void FDialogueLineChatHistory::SetSystemPrompt(const FString& InSystemPrompt)
{
    const auto FormattedPrompt = FString::Format(TEXT("# 시스템 프롬프트\r\n{0}"), {InSystemPrompt});
    const auto Item            = FItem_OpenAI(FInputMessage_OpenAI::CreateSystemTextMessage(FInputTextContent_OpenAI{FormattedPrompt}));
    this->SystemPromptItem     = Item;
}

void FDialogueLineChatHistory::SetEmotionDescriptionItem(const FString& InEmotionDescription)
{
    const auto FormattedEmotion = FString::Format(TEXT("# 이 대사를 말할 때 캐릭터의 언어적/비언어적 표현\r\n{0}"), {InEmotionDescription});
    const auto Item             = FItem_OpenAI{FInputMessage_OpenAI::CreateUserTextMessage(FInputTextContent_OpenAI{FormattedEmotion})};
    this->EmotionDescription    = Item;
}

void FDialogueLineChatHistory::SetDialogueDescriptionItem(const FString& InDialogueDescription)
{
    const auto FormattedDialogueDescription = FString::Format(TEXT("# 이 대화에 대한 전반적인 상황\r\n{0}"), {InDialogueDescription});
    const auto Item                         = FItem_OpenAI{FInputMessage_OpenAI::CreateUserTextMessage(FInputTextContent_OpenAI{FormattedDialogueDescription})};
    this->DialogueDescriptionItem           = Item;
}

void FDialogueLineChatHistory::SetLineDescriptionItem(const FString& InLineDescription)
{
    const auto FormattedLineDescription = FString::Format(TEXT("# 작성할 대사에 대한 설명\r\n{0}"), {InLineDescription});
    const auto Item                     = FItem_OpenAI{FInputMessage_OpenAI::CreateUserTextMessage(FInputTextContent_OpenAI{FormattedLineDescription})};
    this->LineDescriptionItem           = Item;
}

TArray<FItem_OpenAI> FDialogueLineChatHistory::GetBackgroundItems() const
{
    return BackgroundItems;
}

TArray<FItem_OpenAI> FDialogueLineChatHistory::GetHistoryItems() const
{
    TArray<FItem_OpenAI> Items;

    // 시스템 프롬프트를 주입한다.
    if ( SystemPromptItem.IsSet() )
    {
        Items.Add(SystemPromptItem.GetValue());
    }

    // 캐릭터의 말투 성격 이전 경험을 주입한다.
    if ( !BackgroundItems.IsEmpty() )
    {
        Items.Append(BackgroundItems);
    }

    // 대화 전체의 상황에 대한 설명을 담는다.
    if ( DialogueDescriptionItem.IsSet() )
    {
        Items.Add(DialogueDescriptionItem.GetValue());
    }

    // 캐릭터가 말해야 하는 대사에 대한 설명을 담는다.
    if ( LineDescriptionItem.IsSet() )
    {
        Items.Add(LineDescriptionItem.GetValue());
    }

    // 캐릭터가 LineDescriptionItem의 결과를 말할 때 어떤 감정을 갖는지 어떤 언어적, 비언어적 상태인지 나타낸다.
    if ( EmotionDescription.IsSet() )
    {
        Items.Add(EmotionDescription.GetValue());
    }

    // 전체 채팅에 대한 기록을 첨부한다.
    Items.Append(HistoryItems);
    return Items;
}

void FDialogueLineChatHistory::SetBackgroundItems(const TArray<FItem_OpenAI>& InBackgroundItems)
{
    BackgroundItems = InBackgroundItems;
}

TOptional<FItem_OpenAI> FDialogueLineChatHistory::GetSystemPromptItem() const
{
    return SystemPromptItem;
}

TOptional<FItem_OpenAI> FDialogueLineChatHistory::GetEmotionDescription() const
{
    return EmotionDescription;
}

TOptional<FItem_OpenAI> FDialogueLineChatHistory::GetDialogueDescriptionItem() const
{
    return DialogueDescriptionItem;
}

TOptional<FItem_OpenAI> FDialogueLineChatHistory::GetLineDescriptionItem() const
{
    return LineDescriptionItem;
}
