// Copyright notice here

#include "Character/DialogueCharacterAsset.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "Character/DialogueCharacterEditor.h"
#include "Graph/DialogueGraphAsset.h"
#include "Misc/Paths.h"
#include "UObject/ObjectMacros.h"

#define LOCTEXT_NAMESPACE "DialogueCharacterAssetModule"

UDialogueCharacterAssetFactory::UDialogueCharacterAssetFactory(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    SupportedClass = UDialogueCharacterAsset::StaticClass();
    bCreateNew     = true;
    bEditAfterNew  = true;
}

FText UDialogueCharacterAssetFactory::GetDisplayName() const
{
    return LOCTEXT("DialogueCharacterAssetFactory", "Dialogue Character Asset");
}

UObject* UDialogueCharacterAssetFactory::FactoryCreateNew(UClass* InClass,
                                                          UObject* InParent,
                                                          FName InName,
                                                          EObjectFlags Flags,
                                                          UObject* InContext,
                                                          FFeedbackContext* InWarn)
{
    return NewObject<UDialogueCharacterAsset>(InParent, InClass, InName, Flags);
}

bool UDialogueCharacterAssetFactory::ShouldShowInNewMenu() const
{
    return true;
}

uint32 UDialogueCharacterAssetFactory::GetMenuCategories() const
{
    return FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get().FindAdvancedAssetCategory(DialogueAsset::CategoryBase);
}

UClass* FAssetTypeActions_DialogueCharacter::GetSupportedClass() const
{
    return UDialogueCharacterAsset::StaticClass();
}

uint32 FAssetTypeActions_DialogueCharacter::GetCategories()
{
    return FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get().FindAdvancedAssetCategory(DialogueAsset::CategoryBase);
}

void FAssetTypeActions_DialogueCharacter::OpenAssetEditor(const TArray<UObject*>& InObjects,
                                                          TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
    // 편집기에서 열 대화 캐릭터 에셋 선택
    const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ?
                                        EToolkitMode::WorldCentric :
                                        EToolkitMode::Standalone;

    for ( UObject* Object : InObjects )
    {
        if ( UDialogueCharacterAsset* DialogueCharacterAsset = Cast<UDialogueCharacterAsset>(Object) )
        {
            // 에디터 생성
            const TSharedRef<FDialogueCharacterEditor> NewDialogueGraphEditor = MakeShared<FDialogueCharacterEditor>();

            // 에디터 초기화
            NewDialogueGraphEditor->InitDialogueCharacterEditor(Mode, EditWithinLevelEditor, DialogueCharacterAsset);
        }
    }
}

#undef LOCTEXT_NAMESPACE
