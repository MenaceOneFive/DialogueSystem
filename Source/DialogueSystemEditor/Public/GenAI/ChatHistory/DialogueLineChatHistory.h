#pragma once

#include "OpenAI/ChatHistory_OpenAI.h"
#include "OpenAI/Type/Item_OpenAI.h"

class FDialogueLineChatHistory final : public FChatHistory_OpenAI
{
public:
    FDialogueLineChatHistory()                   = default;
    virtual ~FDialogueLineChatHistory() override = default;

    void SetSystemPrompt(const FString& InSystemPrompt);
    void SetEmotionDescriptionItem(const FString& InEmotionDescription);
    void SetDialogueDescriptionItem(const FString& InDialogueDescription);
    void SetLineDescriptionItem(const FString& InLineDescription);
    void SetBackgroundItems(const TArray<FItem_OpenAI>& InBackgroundItems);

    TOptional<FItem_OpenAI> GetSystemPromptItem() const;
    TOptional<FItem_OpenAI> GetEmotionDescription() const;
    TOptional<FItem_OpenAI> GetDialogueDescriptionItem() const;
    TOptional<FItem_OpenAI> GetLineDescriptionItem() const;
    TArray<FItem_OpenAI> GetBackgroundItems() const;

    virtual TArray<FItem_OpenAI> GetHistoryItems() const override;

private:
    TOptional<FItem_OpenAI> SystemPromptItem;
    TOptional<FItem_OpenAI> EmotionDescription;
    TOptional<FItem_OpenAI> DialogueDescriptionItem;
    TOptional<FItem_OpenAI> LineDescriptionItem;
    TArray<FItem_OpenAI> BackgroundItems;
};
