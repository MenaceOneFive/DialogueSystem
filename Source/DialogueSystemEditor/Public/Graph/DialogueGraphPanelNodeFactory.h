// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraphUtilities.h"

/**
 * 
 */
class DIALOGUESYSTEMEDITOR_API FDialogueGraphPanelNodeFactory : public FGraphPanelNodeFactory
{
public:
    FDialogueGraphPanelNodeFactory();
    virtual ~FDialogueGraphPanelNodeFactory() override;
    virtual TSharedPtr<class SGraphNode> CreateNode(UEdGraphNode* Node) const override;
};
