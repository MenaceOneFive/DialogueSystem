// Fill out your copyright notice in the Description page of Project Settings.

// ReSharper disable CppUEBlueprintCallableFunctionUnused

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UObject/Interface.h"
#include "DialogueSelectionContainerWidget.generated.h"

class IDialogueSelectionItemWidget;
class UDialogueSelectionItem;
// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UDialogueSelectionContainerWidget : public UInterface
{
    GENERATED_BODY()
};

/// <summary>
/// 선택지에 대한 표시/숨김, 활성화/비활성화, 설정/해제를 정의합니다.
/// 각 선택지의 위치는 시계와 같다고 생각하면 됩니다.
/// </summary>
class DIALOGUESYSTEMRUNTIME_API IDialogueSelectionContainerWidget
{
    GENERATED_BODY()

    /// TopLeft TopMiddle는 읽을 수는 있는데 생각보다 햇갈려서 시간을 메서드 이름에 기입했습니다.

public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void ResetAllSelectionItem();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void ShowSelectionWidget();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void HideSelectionWidget();

    /// <summary>
    /// 1시 30분 방향 선택지를 활성화합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Enable_1_30_SelectionItem();

    /// <summary>
    /// 1시 30분 방향 선택지를 비활성화합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Disable_1_30_SelectionItem();

    /// <summary>
    /// 1시 30분 방향 선택지를 표시합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Show_1_30_SelectionItem();

    /// <summary>
    /// 1시 30분 방향 선택지를 숨깁니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Hide_1_30_SelectionItem();

    /// <summary>
    /// 1시 30분 방향 선택지를 설정합니다.
    /// </summary>
    /// <param name="SelectionItemParam"></param>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Set_1_30_SelectionItem(const UDialogueSelectionItem* SelectionItemParam);

    /// <summary>
    /// 1시 30분 방향 선택지를 초기화합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Reset_1_30_SelectionItem();


    /// <summary>
    /// 3시 방향 선택지를 활성화합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Enable_3_00_SelectionItem();

    /// <summary>
    /// 3시 방향 선택지를 비활성화합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Disable_3_00_SelectionItem();

    /// <summary>
    /// 3시 방향 선택지를 표시합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Show_3_00_SelectionItem();

    /// <summary>
    /// 3시 방향 선택지를 숨깁니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Hide_3_00_SelectionItem();

    /// <summary>
    /// 3시 방향 선택지를 설정합니다.
    /// </summary>
    /// <param name="SelectionItemParam"></param>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Set_3_00_SelectionItem(const UDialogueSelectionItem* SelectionItemParam);

    /// <summary>
    /// 3시 방향 선택지를 초기화합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Reset_3_00_SelectionItem();


    /// <summary>
    /// 4시 30분 방향 선택지를 활성화합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Enable_4_30_SelectionItem();

    /// <summary>
    /// 4시 30분 방향 선택지를 비활성화합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Disable_4_30_SelectionItem();

    /// <summary>
    /// 4시 30분 방향 선택지를 표시합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Show_4_30_SelectionItem();

    /// <summary>
    /// 4시 30분 방향 선택지를 숨깁니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Hide_4_30_SelectionItem();

    /// <summary>
    /// 4시 30분 방향 선택지를 설정합니다.
    /// </summary>
    /// <param name="SelectionItemParam"></param>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Set_4_30_SelectionItem(const UDialogueSelectionItem* SelectionItemParam);

    /// <summary>
    /// 4시 30분 방향 선택지를 초기화합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Reset_4_30_SelectionItem();


    /// <summary>
    /// 6시 방향 선택지를 활성화합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Enable_6_00_SelectionItem();

    /// <summary>
    /// 6시 방향 선택지를 비활성화합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Disable_6_00_SelectionItem();

    /// <summary>
    /// 6시 방향 선택지를 표시합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Show_6_00_SelectionItem();

    /// <summary>
    /// 6시 방향 선택지를 숨깁니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Hide_6_00_SelectionItem();

    /// <summary>
    /// 6시 방향 선택지를 설정합니다.
    /// </summary>
    /// <param name="SelectionItemParam"></param>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Set_6_00_SelectionItem(const UDialogueSelectionItem* SelectionItemParam);

    /// <summary>
    /// 6시 방향 선택지를 초기화합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Reset_6_00_SelectionItem();


    /// <summary>
    /// 7시 30분 방향 선택지를 활성화합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Enable_7_30_SelectionItem();

    /// <summary>
    /// 7시 30분 방향 선택지를 비활성화합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Disable_7_30_SelectionItem();

    /// <summary>
    /// 7시 30분 방향 선택지를 표시합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Show_7_30_SelectionItem();

    /// <summary>
    /// 7시 30분 방향 선택지를 숨깁니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Hide_7_30_SelectionItem();

    /// <summary>
    /// 7시 30분 방향 선택지를 설정합니다.
    /// </summary>
    /// <param name="SelectionItemParam"></param>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Set_7_30_SelectionItem(const UDialogueSelectionItem* SelectionItemParam);

    /// <summary>
    /// 7시 30분 방향 선택지를 초기화합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Reset_7_30_SelectionItem();


    /// <summary>
    /// 9시 방향 선택지를 활성화합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Enable_9_00_SelectionItem();

    /// <summary>
    /// 9시 방향 선택지를 비활성화합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Disable_9_00_SelectionItem();

    /// <summary>
    /// 9시 방향 선택지를 표시합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Show_9_00_SelectionItem();

    /// <summary>
    /// 9시 방향 선택지를 숨깁니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Hide_9_00_SelectionItem();

    /// <summary>
    /// 9시 방향 선택지를 설정합니다.
    /// </summary>
    /// <param name="SelectionItemParam"></param>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Set_9_00_SelectionItem(const UDialogueSelectionItem* SelectionItemParam);

    /// <summary>
    /// 9시 방향 선택지를 초기화합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Reset_9_00_SelectionItem();


    /// <summary>
    /// 10시 30분 방향 선택지를 활성화합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Enable_10_30_SelectionItem();

    /// <summary>
    /// 10시 30분 방향 선택지를 비활성화합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Disable_10_30_SelectionItem();

    /// <summary>
    /// 10시 30분 방향 선택지를 표시합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Show_10_30_SelectionItem();

    /// <summary>
    /// 10시 30분 방향 선택지를 숨깁니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Hide_10_30_SelectionItem();

    /// <summary>
    /// 10시 30분 방향 선택지를 설정합니다.
    /// </summary>
    /// <param name="SelectionItemParam"></param>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Set_10_30_SelectionItem(const UDialogueSelectionItem* SelectionItemParam);

    /// <summary>
    /// 10시 30분 방향 선택지를 초기화합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Reset_10_30_SelectionItem();


    /// <summary>
    /// 12시 방향 선택지를 활성화합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Enable_12_00_SelectionItem();

    /// <summary>
    /// 12시 방향 선택지를 비활성화합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Disable_12_00_SelectionItem();

    /// <summary>
    /// 12시 방향 선택지를 표시합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Show_12_00_SelectionItem();

    /// <summary>
    /// 12시 방향 선택지를 숨깁니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Hide_12_00_SelectionItem();

    /// <summary>
    /// 12시 방향 선택지를 설정합니다.
    /// </summary>
    /// <param name="SelectionItemParam"></param>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Set_12_00_SelectionItem(const UDialogueSelectionItem* SelectionItemParam);

    /// <summary>
    /// 12시 방향 선택지를 초기화합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Reset_12_00_SelectionItem();

protected:
    /// <summary>
    /// 1시 30분 방향 선택지를 반환합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    TScriptInterface<IDialogueSelectionItemWidget> Get_1_30_SelectionItem();

    /// <summary>
    /// 3시 방향 선택지를 반환합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    TScriptInterface<IDialogueSelectionItemWidget> Get_3_00_SelectionItem();

    /// <summary>
    /// 4시 30분 방향 선택지를 반환합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    TScriptInterface<IDialogueSelectionItemWidget> Get_4_30_SelectionItem();

    /// <summary>
    /// 6시 방향 선택지를 반환합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    TScriptInterface<IDialogueSelectionItemWidget> Get_6_00_SelectionItem();

    /// <summary>
    /// 7시 30분 방향 선택지를 반환합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    TScriptInterface<IDialogueSelectionItemWidget> Get_7_30_SelectionItem();

    /// <summary>
    /// 9시 방향 선택지를 반환합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    TScriptInterface<IDialogueSelectionItemWidget> Get_9_00_SelectionItem();

    /// <summary>
    /// 10시 30분 방향 선택지를 반환합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    TScriptInterface<IDialogueSelectionItemWidget> Get_10_30_SelectionItem();

    /// <summary>
    /// 12시 방향 선택지를 반환합니다.
    /// </summary>
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    TScriptInterface<IDialogueSelectionItemWidget> Get_12_00_SelectionItem();
};


UCLASS(Blueprintable)
class DIALOGUESYSTEMRUNTIME_API UDialogueSelectionContainerWidgetImpl : public UUserWidget,
                                                                        public IDialogueSelectionContainerWidget
{
    GENERATED_BODY()

public:
    /// <summary>
    /// 1시 30분 방향 선택지를 활성화하는 기본 구현입니다.
    /// </summary>
    virtual void Enable_1_30_SelectionItem_Implementation() override;

    /// <summary>
    /// 1시 30분 방향 선택지를 비활성화하는 기본 구현입니다.
    /// </summary>
    virtual void Disable_1_30_SelectionItem_Implementation() override;

    /// <summary>
    /// 1시 30분 방향 선택지를 표시하는 기본 구현입니다.
    /// </summary>
    virtual void Show_1_30_SelectionItem_Implementation() override;

    /// <summary>
    /// 1시 30분 방향 선택지를 숨기는 기본 구현입니다.
    /// </summary>
    virtual void Hide_1_30_SelectionItem_Implementation() override;

    /// <summary>
    /// 1시 30분 방향 선택지를 설정하는 기본 구현입니다.
    /// <param name="InSelectionItem">설정할 선택지 데이터입니다.</param>
    /// </summary>
    virtual void Set_1_30_SelectionItem_Implementation(const UDialogueSelectionItem* InSelectionItem) override;

    /// <summary>
    /// 1시 30분 방향 선택지를 초기화하는 기본 구현입니다.
    /// </summary>
    virtual void Reset_1_30_SelectionItem_Implementation() override;

    /// <summary>
    /// 3시 방향 선택지를 활성화하는 기본 구현입니다.
    /// </summary>
    virtual void Enable_3_00_SelectionItem_Implementation() override;

    /// <summary>
    /// 3시 방향 선택지를 비활성화하는 기본 구현입니다.
    /// </summary>
    virtual void Disable_3_00_SelectionItem_Implementation() override;

    /// <summary>
    /// 3시 방향 선택지를 표시하는 기본 구현입니다.
    /// </summary>
    virtual void Show_3_00_SelectionItem_Implementation() override;

    /// <summary>
    /// 3시 방향 선택지를 숨기는 기본 구현입니다.
    /// </summary>
    virtual void Hide_3_00_SelectionItem_Implementation() override;

    /// <summary>
    /// 3시 방향 선택지를 설정하는 기본 구현입니다.
    /// <param name="InSelectionItem">설정할 선택지 데이터입니다.</param>
    /// </summary>
    virtual void Set_3_00_SelectionItem_Implementation(const UDialogueSelectionItem* InSelectionItem) override;

    /// <summary>
    /// 3시 방향 선택지를 초기화하는 기본 구현입니다.
    /// </summary>
    virtual void Reset_3_00_SelectionItem_Implementation() override;

    /// <summary>
    /// 4시 30분 방향 선택지를 활성화하는 기본 구현입니다.
    /// </summary>
    virtual void Enable_4_30_SelectionItem_Implementation() override;

    /// <summary>
    /// 4시 30분 방향 선택지를 비활성화하는 기본 구현입니다.
    /// </summary>
    virtual void Disable_4_30_SelectionItem_Implementation() override;

    /// <summary>
    /// 4시 30분 방향 선택지를 표시하는 기본 구현입니다.
    /// </summary>
    virtual void Show_4_30_SelectionItem_Implementation() override;

    /// <summary>
    /// 4시 30분 방향 선택지를 숨기는 기본 구현입니다.
    /// </summary>
    virtual void Hide_4_30_SelectionItem_Implementation() override;

    /// <summary>
    /// 4시 30분 방향 선택지를 설정하는 기본 구현입니다.
    /// <param name="InSelectionItem">설정할 선택지 데이터입니다.</param>
    /// </summary>
    virtual void Set_4_30_SelectionItem_Implementation(const UDialogueSelectionItem* InSelectionItem) override;

    /// <summary>
    /// 4시 30분 방향 선택지를 초기화하는 기본 구현입니다.
    /// </summary>
    virtual void Reset_4_30_SelectionItem_Implementation() override;

    /// <summary>
    /// 6시 방향 선택지를 활성화하는 기본 구현입니다.
    /// </summary>
    virtual void Enable_6_00_SelectionItem_Implementation() override;

    /// <summary>
    /// 6시 방향 선택지를 비활성화하는 기본 구현입니다.
    /// </summary>
    virtual void Disable_6_00_SelectionItem_Implementation() override;

    /// <summary>
    /// 6시 방향 선택지를 표시하는 기본 구현입니다.
    /// </summary>
    virtual void Show_6_00_SelectionItem_Implementation() override;

    /// <summary>
    /// 6시 방향 선택지를 숨기는 기본 구현입니다.
    /// </summary>
    virtual void Hide_6_00_SelectionItem_Implementation() override;

    /// <summary>
    /// 6시 방향 선택지를 설정하는 기본 구현입니다.
    /// <param name="InSelectionItem">설정할 선택지 데이터입니다.</param>
    /// </summary>
    virtual void Set_6_00_SelectionItem_Implementation(const UDialogueSelectionItem* InSelectionItem) override;

    /// <summary>
    /// 6시 방향 선택지를 초기화하는 기본 구현입니다.
    /// </summary>
    virtual void Reset_6_00_SelectionItem_Implementation() override;

    /// <summary>
    /// 7시 30분 방향 선택지를 활성화하는 기본 구현입니다.
    /// </summary>
    virtual void Enable_7_30_SelectionItem_Implementation() override;

    /// <summary>
    /// 7시 30분 방향 선택지를 비활성화하는 기본 구현입니다.
    /// </summary>
    virtual void Disable_7_30_SelectionItem_Implementation() override;

    /// <summary>
    /// 7시 30분 방향 선택지를 표시하는 기본 구현입니다.
    /// </summary>
    virtual void Show_7_30_SelectionItem_Implementation() override;

    /// <summary>
    /// 7시 30분 방향 선택지를 숨기는 기본 구현입니다.
    /// </summary>
    virtual void Hide_7_30_SelectionItem_Implementation() override;

    /// <summary>
    /// 7시 30분 방향 선택지를 설정하는 기본 구현입니다.
    /// <param name="InSelectionItem">설정할 선택지 데이터입니다.</param>
    /// </summary>
    virtual void Set_7_30_SelectionItem_Implementation(const UDialogueSelectionItem* InSelectionItem) override;

    /// <summary>
    /// 7시 30분 방향 선택지를 초기화하는 기본 구현입니다.
    /// </summary>
    virtual void Reset_7_30_SelectionItem_Implementation() override;

    /// <summary>
    /// 9시 방향 선택지를 활성화하는 기본 구현입니다.
    /// </summary>
    virtual void Enable_9_00_SelectionItem_Implementation() override;

    /// <summary>
    /// 9시 방향 선택지를 비활성화하는 기본 구현입니다.
    /// </summary>
    virtual void Disable_9_00_SelectionItem_Implementation() override;

    /// <summary>
    /// 9시 방향 선택지를 표시하는 기본 구현입니다.
    /// </summary>
    virtual void Show_9_00_SelectionItem_Implementation() override;

    /// <summary>
    /// 9시 방향 선택지를 숨기는 기본 구현입니다.
    /// </summary>
    virtual void Hide_9_00_SelectionItem_Implementation() override;

    /// <summary>
    /// 9시 방향 선택지를 설정하는 기본 구현입니다.
    /// <param name="InSelectionItem">설정할 선택지 데이터입니다.</param>
    /// </summary>
    virtual void Set_9_00_SelectionItem_Implementation(const UDialogueSelectionItem* InSelectionItem) override;

    /// <summary>
    /// 9시 방향 선택지를 초기화하는 기본 구현입니다.
    /// </summary>
    virtual void Reset_9_00_SelectionItem_Implementation() override;

    /// <summary>
    /// 10시 30분 방향 선택지를 활성화하는 기본 구현입니다.
    /// </summary>
    virtual void Enable_10_30_SelectionItem_Implementation() override;

    /// <summary>
    /// 10시 30분 방향 선택지를 비활성화하는 기본 구현입니다.
    /// </summary>
    virtual void Disable_10_30_SelectionItem_Implementation() override;

    /// <summary>
    /// 10시 30분 방향 선택지를 표시하는 기본 구현입니다.
    /// </summary>
    virtual void Show_10_30_SelectionItem_Implementation() override;

    /// <summary>
    /// 10시 30분 방향 선택지를 숨기는 기본 구현입니다.
    /// </summary>
    virtual void Hide_10_30_SelectionItem_Implementation() override;

    /// <summary>
    /// 10시 30분 방향 선택지를 설정하는 기본 구현입니다.
    /// <param name="InSelectionItem">설정할 선택지 데이터입니다.</param>
    /// </summary>
    virtual void Set_10_30_SelectionItem_Implementation(const UDialogueSelectionItem* InSelectionItem) override;

    /// <summary>
    /// 10시 30분 방향 선택지를 초기화하는 기본 구현입니다.
    /// </summary>
    virtual void Reset_10_30_SelectionItem_Implementation() override;

    /// <summary>
    /// 12시 방향 선택지를 활성화하는 기본 구현입니다.
    /// </summary>
    virtual void Enable_12_00_SelectionItem_Implementation() override;

    /// <summary>
    /// 12시 방향 선택지를 비활성화하는 기본 구현입니다.
    /// </summary>
    virtual void Disable_12_00_SelectionItem_Implementation() override;

    /// <summary>
    /// 12시 방향 선택지를 표시하는 기본 구현입니다.
    /// </summary>
    virtual void Show_12_00_SelectionItem_Implementation() override;

    /// <summary>
    /// 12시 방향 선택지를 숨기는 기본 구현입니다.
    /// </summary>
    virtual void Hide_12_00_SelectionItem_Implementation() override;

    /// <summary>
    /// 12시 방향 선택지를 설정하는 기본 구현입니다.
    /// <param name="InSelectionItem">설정할 선택지 데이터입니다.</param>
    /// </summary>
    virtual void Set_12_00_SelectionItem_Implementation(const UDialogueSelectionItem* InSelectionItem) override;

    /// <summary>
    /// 12시 방향 선택지를 초기화하는 기본 구현입니다.
    /// </summary>
    virtual void Reset_12_00_SelectionItem_Implementation() override;
};
