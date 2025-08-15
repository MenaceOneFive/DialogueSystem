// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraphUtilities.h"


class FDialogueSystemEditorModule;

class DIALOGUESYSTEMEDITOR_API FDialogueGraphPanelNodeFactory final : public FGraphPanelNodeFactory
{
public:
    FDialogueGraphPanelNodeFactory();
    virtual ~FDialogueGraphPanelNodeFactory() override;

    virtual TSharedPtr<SGraphNode> CreateNode(UEdGraphNode* Node) const override;

private:
    FDialogueSystemEditorModule* Module;
};
