// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"
#include "Factories/Factory.h"
#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"
#include "DialogueCharacterAsset.generated.h"

namespace DialogueAsset
{
    inline FName CategoryCharacter = "DialogueSystem/Character";
}

UCLASS(BlueprintType)
class DIALOGUESYSTEMEDITOR_API UDialogueCharacterAsset : public UObject
{
    GENERATED_BODY()

public:
    // 캐릭터의 이름
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Character")
    FString CharacterName;

    // 캐릭터에 대한 설명
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Character")
    FString Description;

    // 기타 캐릭터 관련 속성들 추가 가능
};

/** Factory for Dialogue Character Asset */
UCLASS()
class DIALOGUESYSTEMEDITOR_API UDialogueCharacterAssetFactory : public UFactory
{
    GENERATED_BODY()

public:
    explicit UDialogueCharacterAssetFactory(const FObjectInitializer& ObjectInitializer);
    virtual FText GetDisplayName() const override;
    virtual UObject* FactoryCreateNew(UClass* InClass,
                                      UObject* InParent,
                                      FName InName,
                                      EObjectFlags Flags,
                                      UObject* Context,
                                      FFeedbackContext* Warn) override;
    virtual bool ShouldShowInNewMenu() const override;
    virtual uint32 GetMenuCategories() const override;
};

/** AssetTypeActions for Dialogue Character Asset */
class FAssetTypeActions_DialogueCharacter final : public FAssetTypeActions_Base
{
public:
    virtual FText GetName() const override { return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_DialogueCharacter", "Dialogue Character"); }
    virtual FColor GetTypeColor() const override { return FColor(0, 128, 255); }
    virtual UClass* GetSupportedClass() const override;
    virtual uint32 GetCategories() override;
    virtual void OpenAssetEditor(const TArray<UObject*>& InObjects,
                                 TSharedPtr<IToolkitHost> EditWithinLevelEditor) override;
};
