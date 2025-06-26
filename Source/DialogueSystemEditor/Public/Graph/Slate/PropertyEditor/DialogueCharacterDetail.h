// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


class FDialogueCharacterDetailViewModel;
class SMultiLineEditableTextBox;

class DIALOGUESYSTEMEDITOR_API FDialogueCharacterDetail final : public IPropertyTypeCustomization
{
public:
    FDialogueCharacterDetail()                   = default;
    virtual ~FDialogueCharacterDetail() override = default;

    static TSharedRef<IPropertyTypeCustomization> MakeInstance();

    virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle,
                                 FDetailWidgetRow& HeaderRow,
                                 IPropertyTypeCustomizationUtils& CustomizationUtils) override;

    virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
                                   IDetailChildrenBuilder& ChildBuilder,
                                   IPropertyTypeCustomizationUtils& CustomizationUtils) override;

private:
    void CreateRootWidget(const TSharedRef<IPropertyHandle>& PropertyHandle,
                          IDetailChildrenBuilder& ChildBuilder);
    void CreateCharacterAssetButton();
    void InitializeReadOnlyWidget();

private:
    TSharedPtr<SMultiLineEditableTextBox> NameTextBox;
    TSharedPtr<SMultiLineEditableTextBox> DescriptionTextBox;
    TSharedPtr<SButton> OpenCharacterAssetButton;
    TSharedPtr<FDialogueCharacterDetailViewModel> ViewModel;
};
