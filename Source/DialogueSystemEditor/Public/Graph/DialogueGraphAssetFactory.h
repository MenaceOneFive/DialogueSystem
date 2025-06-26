// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <AssetTypeActions_Base.h>
#include <UObject/Object.h>

#include "DialogueGraphAssetFactory.generated.h"


class UDialogueGraph;
class UDialogueEdGraph;

namespace DialogueAsset
{
    inline FName CategoryBase = "DialogueSystem";
}

UCLASS()
class DIALOGUESYSTEMEDITOR_API UDialogueGraphAssetFactory : public UFactory
{
    GENERATED_BODY()

public:
    explicit UDialogueGraphAssetFactory(const FObjectInitializer& ObjectInitializer);
    virtual FText GetDisplayName() const override;
    virtual UObject* FactoryCreateNew(UClass* InClass,
                                      UObject* InParent,
                                      FName InName,
                                      EObjectFlags Flags,
                                      UObject* Context,
                                      FFeedbackContext* Warn) override;
    UBlueprint* CreateBlueprintForAsset(UDialogueGraph* GraphAsset) const;
    virtual bool ShouldShowInNewMenu() const override;
    virtual uint32 GetMenuCategories() const override;
};


// 대화 그래프 에셋 타입 액션 - 에셋 편집기 열기 등
class FAssetTypeActions_DialogueGraph final : public FAssetTypeActions_Base
{
public:
    // FAssetTypeActions_Base 인터페이스
    virtual FText GetName() const override { return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_DialogueGraph", "Dialogue Graph"); }
    virtual FColor GetTypeColor() const override { return FColor(175, 0, 128); }
    virtual UClass* GetSupportedClass() const override;
    virtual uint32 GetCategories() override;
    virtual void OpenAssetEditor(const TArray<UObject*>& InObjects,
                                 TSharedPtr<IToolkitHost> EditWithinLevelEditor) override;
};
