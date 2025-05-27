// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

/**
 * 
 */
class DIALOGUESYSTEMEDITOR_API FDialogueGraphDetailCustomization : public IDetailCustomization
{
public:
    FDialogueGraphDetailCustomization();
    virtual ~FDialogueGraphDetailCustomization() override;
    static TSharedRef<IDetailCustomization> MakeInstance();

    void OpenRelatedMovieSceneWidget(const IDetailLayoutBuilder& DetailBuilder);
    void CreateTheNumberOfNodeWidget(IDetailLayoutBuilder& DetailBuilder);
    void CreateTheNumberOfLineNodeWidget(IDetailLayoutBuilder& DetailBuilder);
    void CreateDescriptionTextBoxWidget(IDetailLayoutBuilder& DetailBuilder);
    void CreateMemoTextBoxWidget(IDetailLayoutBuilder& DetailBuilder);
    void CreateTheNumberOfSelectNodeWidget(IDetailLayoutBuilder& DetailBuilder);

    virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

protected:
    TSharedPtr<IPropertyHandle> DescriptionHandle;
    TSharedPtr<IPropertyHandle> MemoHandle;
};
