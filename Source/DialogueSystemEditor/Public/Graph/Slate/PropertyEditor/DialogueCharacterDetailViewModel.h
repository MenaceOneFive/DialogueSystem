#pragma once

#include "CoreMinimal.h"
#include "Character/DialogueCharacterAsset.h"

// ViewModel: 데이터와 View 연결, 바인딩 및 커맨드 처리

class FDialogueCharacterDetailViewModel
{
public:
    explicit FDialogueCharacterDetailViewModel(const TSharedPtr<IPropertyHandle>& InPropertyHandle);
    UDialogueCharacterAsset* GetObjectBeingEdited() const;

    // 데이터 바인딩 및 커맨드 함수 선언
    FString GetCharacterName() const;
    void SetCharacterName(const FString& InName) const;
    void SetOnCharacterNameChanged(const FSimpleDelegate& InCharacterNameDelegate);

    FString GetDescription() const;
    void SetDescription(const FString& InDescription) const;
    void SetOnDescriptionChanged(const FSimpleDelegate& InDescriptionDelegate);

    TObjectPtr<UTexture2D> GetCharacterProfileImage() const;

    void OpenCharacterAsset() const;

private:
    // TSharedPtr<IPropertyHandle> NameHandle;
    // TSharedPtr<IPropertyHandle> DescriptionHandle;
    TSharedPtr<IPropertyHandle> PropertyHandle;
    FSimpleDelegate OnCharacterNameChanged;
    FSimpleDelegate OnDescriptionChanged;
};
