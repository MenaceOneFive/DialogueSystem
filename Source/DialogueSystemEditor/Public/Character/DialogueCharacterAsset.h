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

/// <summary>
/// 캐릭터 설정집 에셋
/// </summary>
UCLASS(BlueprintType)
class DIALOGUESYSTEMEDITOR_API UDialogueCharacterAsset : public UObject
{
    GENERATED_BODY()

public:
    // 캐릭터의 프로필 이미지
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Character")
    TObjectPtr<UTexture2D> CharacterProfileImage;

    // 캐릭터의 이름
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Character")
    FString Name;

    // 캐릭터에 대한 설명
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Character")
    FString Description;

public:
    // 기타 캐릭터 관련 속성들 추가 가능
};

/// <summary>
/// 캐릭터 에셋의 팩토리 클래스
/// </summary>
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

/// <summary>
/// 캐릭터 에셋에 대해 AssetTypeAction를 구현
/// </summary>
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

/// <summary>
/// 에셋의 썸네일을 캐릭터의 프로필 이미지로 바꾸는 기능을 구현함
/// </summary>
UCLASS()
class DIALOGUESYSTEMEDITOR_API UDialogueCharacterThumbnailRenderer : public UThumbnailRenderer
{
    GENERATED_BODY()

public:
    virtual void Draw(UObject* Object,
                      int32 X,
                      int32 Y,
                      uint32 Width,
                      uint32 Height,
                      FRenderTarget* Viewport,
                      FCanvas* Canvas,
                      bool bAdditionalViewFamily) override;
};
