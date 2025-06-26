// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IDetailCustomization.h"
#include "Graph/Node/DialogueEdGraphNode.h"
#include "Graph/DialogueGraphAssetFactory.h"
#include "Graph/Slate/DialogueNodeDetail.h"
#include "OpenAI/ChatHistory_OpenAI.h"

#define LOCTEXT_NAMESPACE "DialogueLineDetails"

class FOpenAIChatSession;
class FChatSessionBase;
class FGeminiChatSession;

class SChatBox;
class SWizardWidget;
class UGenerateDialogueLineWindowContents;
class FGenerateDialogueLineWindow;

class DIALOGUESYSTEMEDITOR_API FDialogueLineNodeDetail final : public FDialogueNodeDetail
{
public:
    static TSharedRef<IDetailCustomization> MakeInstance();

public:
    virtual ~FDialogueLineNodeDetail() override;
    virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

private:
    void OnDialogueLinePropertyChanged(TSharedPtr<IPropertyHandle> PropertyHandle);
    void CreateDialogueLineTextBox(IDetailLayoutBuilder& DetailBuilder);
    FString GetDescription(IDetailLayoutBuilder& DetailBuilder) const;
    FDialogueLineNodeDetail() = default;

    FString DialogueLineTextContent;

private:
    TWeakObjectPtr<UDialogueEdGraphDialogueLineNode> DialogueLineNodeToEdit;
    bool bShowGenerationCategory = false;
    bool bShowPrompt             = false;
};

#undef LOCTEXT_NAMESPACE
