// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ChatSessionBase.h"
#include "IDetailCustomization.h"
#include "Graph/DialogueEdGraphNodes.h"
#include "Graph/DialogueGraphAsset.h"
#include "OpenAI/ChatHistory_OpenAI.h"


#define LOCTEXT_NAMESPACE "DialogueLineDetails"

class FOpenAIChatSession;
class FChatSessionBase;
class FGeminiChatSession;

class SChatBox;
class SWizardWidget;
class UGenerateDialogueLineWindowContents;
class FGenerateDialogueLineWindow;


class FDialogueEdDialogueLineNodeDetail final : public IDetailCustomization
{
public:
    static TSharedRef<IDetailCustomization> MakeInstance();

public:
    virtual ~FDialogueEdDialogueLineNodeDetail() override;

    virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

private:
    void OnDialogueLinePropertyChanged(TSharedPtr<IPropertyHandle> PropertyHandle);
    void CreateDialogueLineTextBox(IDetailLayoutBuilder& DetailBuilder);
    FString GetDescription(IDetailLayoutBuilder& DetailBuilder);
    FDialogueEdDialogueLineNodeDetail() = default;

    FString DialogueLineTextContent;

private:
    TWeakObjectPtr<UDialogueEdGraphDialogueLineNode> DialogueLineNodeToEdit;
    bool bShowGenerationCategory = false;
    bool bShowPrompt             = false;
};

#undef LOCTEXT_NAMESPACE
