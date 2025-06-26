// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/DialogueSelectionContainerWidget.h"

#include "Widget/DialogueSelectionItemWidget.h"


// 1시 30분 방향 선택지를 활성화하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Enable_1_30_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_1_30_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_EnableThisItem(SelectionItem.GetObject());
    }
}

// 1시 30분 방향 선택지를 비활성화하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Disable_1_30_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_1_30_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_DisableThisItem(SelectionItem.GetObject());
    }
}

// 1시 30분 방향 선택지를 표시하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Show_1_30_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_1_30_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_ShowThisItem(SelectionItem.GetObject());
    }
}

// 1시 30분 방향 선택지를 숨기는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Hide_1_30_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_1_30_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_HideThisItem(SelectionItem.GetObject());
    }
}

// 1시 30분 방향 선택지를 설정하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Set_1_30_SelectionItem_Implementation(const UDialogueSelectionItem* InSelectionItem)
{
    if ( const auto SelectionItem = Execute_Get_1_30_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_SetupSelectionItem(SelectionItem.GetObject(), InSelectionItem);
    }
}

// 1시 30분 방향 선택지를 초기화하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Reset_1_30_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_1_30_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_ResetThisItem(SelectionItem.GetObject());
    }
}

// 3시 방향 선택지를 활성화하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Enable_3_00_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_3_00_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_EnableThisItem(SelectionItem.GetObject());
    }
}

// 3시 방향 선택지를 비활성화하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Disable_3_00_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_3_00_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_DisableThisItem(SelectionItem.GetObject());
    }
}

// 3시 방향 선택지를 표시하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Show_3_00_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_3_00_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_ShowThisItem(SelectionItem.GetObject());
    }
}

// 3시 방향 선택지를 숨기는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Hide_3_00_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_3_00_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_HideThisItem(SelectionItem.GetObject());
    }
}

// 3시 방향 선택지를 설정하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Set_3_00_SelectionItem_Implementation(const UDialogueSelectionItem* InSelectionItem)
{
    if ( const auto SelectionItem = Execute_Get_3_00_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_SetupSelectionItem(SelectionItem.GetObject(), InSelectionItem);
    }
}

// 3시 방향 선택지를 초기화하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Reset_3_00_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_3_00_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_ResetThisItem(SelectionItem.GetObject());
    }
}

// 4시 30분 방향 선택지를 활성화하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Enable_4_30_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_4_30_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_EnableThisItem(SelectionItem.GetObject());
    }
}

// 4시 30분 방향 선택지를 비활성화하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Disable_4_30_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_4_30_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_DisableThisItem(SelectionItem.GetObject());
    }
}

// 4시 30분 방향 선택지를 표시하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Show_4_30_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_4_30_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_ShowThisItem(SelectionItem.GetObject());
    }
}

// 4시 30분 방향 선택지를 숨기는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Hide_4_30_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_4_30_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_HideThisItem(SelectionItem.GetObject());
    }
}

// 4시 30분 방향 선택지를 설정하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Set_4_30_SelectionItem_Implementation(const UDialogueSelectionItem* InSelectionItem)
{
    if ( const auto SelectionItem = Execute_Get_4_30_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_SetupSelectionItem(SelectionItem.GetObject(), InSelectionItem);
    }
}

// 4시 30분 방향 선택지를 초기화하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Reset_4_30_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_4_30_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_ResetThisItem(SelectionItem.GetObject());
    }
}

// 6시 방향 선택지를 활성화하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Enable_6_00_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_6_00_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_EnableThisItem(SelectionItem.GetObject());
    }
}

// 6시 방향 선택지를 비활성화하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Disable_6_00_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_6_00_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_DisableThisItem(SelectionItem.GetObject());
    }
}

// 6시 방향 선택지를 표시하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Show_6_00_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_6_00_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_ShowThisItem(SelectionItem.GetObject());
    }
}

// 6시 방향 선택지를 숨기는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Hide_6_00_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_6_00_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_HideThisItem(SelectionItem.GetObject());
    }
}

// 6시 방향 선택지를 설정하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Set_6_00_SelectionItem_Implementation(const UDialogueSelectionItem* InSelectionItem)
{
    if ( const auto SelectionItem = Execute_Get_6_00_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_SetupSelectionItem(SelectionItem.GetObject(), InSelectionItem);
    }
}

// 6시 방향 선택지를 초기화하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Reset_6_00_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_6_00_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_ResetThisItem(SelectionItem.GetObject());
    }
}

// 7시 30분 방향 선택지를 활성화하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Enable_7_30_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_7_30_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_EnableThisItem(SelectionItem.GetObject());
    }
}

// 7시 30분 방향 선택지를 비활성화하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Disable_7_30_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_7_30_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_DisableThisItem(SelectionItem.GetObject());
    }
}

// 7시 30분 방향 선택지를 표시하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Show_7_30_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_7_30_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_ShowThisItem(SelectionItem.GetObject());
    }
}

// 7시 30분 방향 선택지를 숨기는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Hide_7_30_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_7_30_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_HideThisItem(SelectionItem.GetObject());
    }
}

// 7시 30분 방향 선택지를 설정하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Set_7_30_SelectionItem_Implementation(const UDialogueSelectionItem* InSelectionItem)
{
    if ( const auto SelectionItem = Execute_Get_7_30_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_SetupSelectionItem(SelectionItem.GetObject(), InSelectionItem);
    }
}

// 7시 30분 방향 선택지를 초기화하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Reset_7_30_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_7_30_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_ResetThisItem(SelectionItem.GetObject());
    }
}

// 9시 방향 선택지를 활성화하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Enable_9_00_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_9_00_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_EnableThisItem(SelectionItem.GetObject());
    }
}

// 9시 방향 선택지를 비활성화하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Disable_9_00_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_9_00_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_DisableThisItem(SelectionItem.GetObject());
    }
}

// 9시 방향 선택지를 표시하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Show_9_00_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_9_00_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_ShowThisItem(SelectionItem.GetObject());
    }
}

// 9시 방향 선택지를 숨기는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Hide_9_00_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_9_00_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_HideThisItem(SelectionItem.GetObject());
    }
}

// 9시 방향 선택지를 설정하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Set_9_00_SelectionItem_Implementation(const UDialogueSelectionItem* InSelectionItem)
{
    if ( const auto SelectionItem = Execute_Get_9_00_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_SetupSelectionItem(SelectionItem.GetObject(), InSelectionItem);
    }
}

// 9시 방향 선택지를 초기화하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Reset_9_00_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_9_00_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_ResetThisItem(SelectionItem.GetObject());
    }
}

// 10시 30분 방향 선택지를 활성화하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Enable_10_30_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_10_30_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_EnableThisItem(SelectionItem.GetObject());
    }
}

// 10시 30분 방향 선택지를 비활성화하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Disable_10_30_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_10_30_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_DisableThisItem(SelectionItem.GetObject());
    }
}

// 10시 30분 방향 선택지를 표시하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Show_10_30_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_10_30_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_ShowThisItem(SelectionItem.GetObject());
    }
}

// 10시 30분 방향 선택지를 숨기는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Hide_10_30_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_10_30_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_HideThisItem(SelectionItem.GetObject());
    }
}

// 10시 30분 방향 선택지를 설정하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Set_10_30_SelectionItem_Implementation(const UDialogueSelectionItem* InSelectionItem)
{
    if ( const auto SelectionItem = Execute_Get_10_30_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_SetupSelectionItem(SelectionItem.GetObject(), InSelectionItem);
    }
}

// 10시 30분 방향 선택지를 초기화하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Reset_10_30_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_10_30_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_ResetThisItem(SelectionItem.GetObject());
    }
}

// 12시 방향 선택지를 활성화하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Enable_12_00_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_12_00_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_EnableThisItem(SelectionItem.GetObject());
    }
}

// 12시 방향 선택지를 비활성화하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Disable_12_00_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_12_00_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_DisableThisItem(SelectionItem.GetObject());
    }
}

// 12시 방향 선택지를 표시하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Show_12_00_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_12_00_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_ShowThisItem(SelectionItem.GetObject());
    }
}

// 12시 방향 선택지를 숨기는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Hide_12_00_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_12_00_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_HideThisItem(SelectionItem.GetObject());
    }
}

// 12시 방향 선택지를 설정하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Set_12_00_SelectionItem_Implementation(const UDialogueSelectionItem* InSelectionItem)
{
    if ( const auto SelectionItem = Execute_Get_12_00_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_SetupSelectionItem(SelectionItem.GetObject(), InSelectionItem);
    }
}

// 12시 방향 선택지를 초기화하는 기본 구현입니다.
void UDialogueSelectionContainerWidgetImpl::Reset_12_00_SelectionItem_Implementation()
{
    if ( const auto SelectionItem = Execute_Get_12_00_SelectionItem(this) )
    {
        IDialogueSelectionItemWidget::Execute_ResetThisItem(SelectionItem.GetObject());
    }
}
