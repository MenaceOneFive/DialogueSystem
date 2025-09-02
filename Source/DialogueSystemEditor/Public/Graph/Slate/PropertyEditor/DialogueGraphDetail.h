// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

class SMultiLineEditableTextBox;
class FDialogueGraphDetailViewModel;
/**
 * 
 */
class DIALOGUESYSTEMEDITOR_API FDialogueGraphDetail final : public IDetailCustomization
{
public:
    // 생성자 / 소멸자
    static TSharedRef<IDetailCustomization> MakeInstance();

    void CreateRootWidget(IDetailLayoutBuilder& DetailBuilder);

    FDialogueGraphDetail() = default;

    virtual ~FDialogueGraphDetail() override = default;

    // 자식 위젯
    void CreateRelatedMovieSceneWidget(IDetailLayoutBuilder& DetailBuilder);

    void CreateTheNumberOfNodeWidget(IDetailLayoutBuilder& DetailBuilder) const;

    void CreateTheNumberOfLineNodeWidget(IDetailLayoutBuilder&) const;

    void CreateDescriptionTextBoxWidget(IDetailLayoutBuilder& DetailBuilder);

    void CreateMemoTextBoxWidget(IDetailLayoutBuilder& DetailBuilder);

    void CreateTheNumberOfSelectNodeWidget(IDetailLayoutBuilder& DetailBuilder) const;

    // IDetailCustomization
    virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

    FReply OpenBlueprintEditor() const;

protected:
    TSharedPtr<FDialogueGraphDetailViewModel> ViewModel;
    TSharedPtr<SMultiLineEditableTextBox>     DescriptionTextBox;
    TSharedPtr<SMultiLineEditableTextBox>     MemoTextBox;
    TSharedPtr<SButton>                       OpenRelatedMovieSceneButton;
};
