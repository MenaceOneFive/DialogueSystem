#include "Graph/Slate/PropertyEditor/DialogueCharacterDetailViewModel.h"

#include "Character/DialogueCharacterAsset.h"

FDialogueCharacterDetailViewModel::FDialogueCharacterDetailViewModel(const TSharedPtr<IPropertyHandle>& InPropertyHandle)
{
    if ( !InPropertyHandle.IsValid() || !InPropertyHandle->IsValidHandle() )
    {
        checkf(false, TEXT("InPropertyHandle is invalid in %s"), TEXT(__FUNCTION__));
        return;
    }
    PropertyHandle = InPropertyHandle;
}

UDialogueCharacterAsset* FDialogueCharacterDetailViewModel::GetObjectBeingEdited() const
{
    if ( !PropertyHandle.IsValid() || !PropertyHandle->IsValidHandle() )
    {
        checkf(false, TEXT("PropertyHandle is invalid in %s"), TEXT(__FUNCTION__));
        return nullptr;
    }

    UObject* ObjectBeingEdited = nullptr;
    const auto Result          = PropertyHandle->GetValue(ObjectBeingEdited);
    if ( Result != FPropertyAccess::Success )
    {
        checkf(false, TEXT("Failed to get value from PropertyHandle in %s"), TEXT(__FUNCTION__));
        return nullptr;
    }

    UDialogueCharacterAsset* CharacterAsset = Cast<UDialogueCharacterAsset>(ObjectBeingEdited);
    if ( !CharacterAsset )
    {
        UE_LOG(LogTemp, Error, TEXT("ObjectBeingEdited is not a UDialogueCharacterAsset in %s"), *FString(__FUNCTION__));
        return nullptr;
    }
    return CharacterAsset;
}

FString FDialogueCharacterDetailViewModel::GetCharacterName() const
{
    if ( auto Object = GetObjectBeingEdited() )
    {
        return Object->Name;
    }
    return "";
}

void FDialogueCharacterDetailViewModel::SetCharacterName(const FString& InName) const
{
    if ( auto Object = GetObjectBeingEdited() )
    {
        Object->Name = InName;
        (void)OnCharacterNameChanged.ExecuteIfBound();
    }
    else
    {
        checkf(false, TEXT("Failed to set character name: ObjectBeingEdited is invalid in %s"), TEXT(__FUNCTION__));
    }
}

void FDialogueCharacterDetailViewModel::SetOnCharacterNameChanged(const FSimpleDelegate& InCharacterNameDelegate)
{
    OnCharacterNameChanged = InCharacterNameDelegate;
}

FString FDialogueCharacterDetailViewModel::GetDescription() const
{
    if ( const auto Object = GetObjectBeingEdited() )
    {
        return Object->Description;
    }

    return "";
}

void FDialogueCharacterDetailViewModel::SetDescription(const FString& InDescription) const
{
    if ( const auto Object = GetObjectBeingEdited() )
    {
        Object->Description = InDescription;
        (void)OnDescriptionChanged.ExecuteIfBound();
    }
    else
    {
        checkf(false, TEXT("Failed to set description: ObjectBeingEdited is invalid in %s"), TEXT(__FUNCTION__));
    }
}

void FDialogueCharacterDetailViewModel::SetOnDescriptionChanged(const FSimpleDelegate& InDescriptionDelegate)
{
    OnDescriptionChanged = InDescriptionDelegate;
}

TObjectPtr<UTexture2D> FDialogueCharacterDetailViewModel::GetCharacterProfileImage() const
{
    if ( const auto Object = GetObjectBeingEdited() )
    {
        return Object->CharacterProfileImage;
    }
    return nullptr;
}

void FDialogueCharacterDetailViewModel::OpenCharacterAsset() const
{
    if ( PropertyHandle.IsValid() && PropertyHandle->IsValidHandle() )
    {
        UObject* ChosenObject;
        PropertyHandle->GetValue(ChosenObject);
        const IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
        if ( const TWeakPtr<IAssetTypeActions> WeakAssetTypeAction = AssetTools.GetAssetTypeActionsForClass(UDialogueCharacterAsset::StaticClass());
            WeakAssetTypeAction.IsValid() &&
            ChosenObject->IsA<UDialogueCharacterAsset>() )
        {
            const TSharedPtr<IAssetTypeActions> AssetTypeAction = WeakAssetTypeAction.Pin();
            AssetTypeAction->OpenAssetEditor({ChosenObject});
        }
    }
}
