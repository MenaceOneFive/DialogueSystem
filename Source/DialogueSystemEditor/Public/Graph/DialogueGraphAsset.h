// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"
#include "DialogueEdGraph.h"
#include "UObject/Object.h"
#include "DialogueGraphAsset.generated.h"

class UDialogueEdGraphDialogueLineNode;
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
    UDialogueGraphAssetFactory(const FObjectInitializer& ObjectInitializer);
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


/** 대화 그래프 에셋 타입 액션 - 에셋 편집기 열기 등 */
class FAssetTypeActions_DialogueGraph : public FAssetTypeActions_Base
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

UCLASS()
class DIALOGUESYSTEMEDITOR_API UDialogueGraphAsset : public UObject
{
    GENERATED_BODY()

public:
    /// 대화 그래프
    UPROPERTY()
    UDialogueEdGraph* DialogueGraph;

    // 에셋 초기화 함수 (새 그래프 생성)
    void InitializeAsset();

    /// <summary>
    /// 이 대화에 대한 설명을 반환합니다.
    /// </summary>
    /// <returns>이 대화에 대한 설명</returns>
    const FString& GetDescription() const;

    /// <summary>
    /// 그래프의 노드 개수를 반환
    /// </summary>
    /// <returns>포함된 그래프의 노드 개수(만약 그래프가 없다면 0을 반환)</returns>
    uint32 CountNode() const;
    uint32 CountSelectNode() const;

    /// <summary>
    /// 그래프의 대화 노드 개수를 반환 
    /// </summary>
    /// <returns> DialogueGraph의 DialogueLineNode 개수</returns>
    uint32 CountLineNode() const;

private:
    // 이 대화의 상황
    UPROPERTY(EditAnywhere)
    FString Description;

    UPROPERTY(EditAnywhere)
    FString Memo;

    /// TODO: 대화에 참여한 캐릭터 목록 반환
};
