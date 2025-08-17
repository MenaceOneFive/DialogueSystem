#include "Graph/Slate/PropertyEditor/DialogueGraphDetailViewModel.h"
#include "Algo/Count.h"
#include "Graph/DialogueEdGraph.h"
#include "Graph/DialogueGraph.h"
#include "Graph/Node/DialogueEdGraphDialogueLineNode.h"
#include "Graph/Node/DialogueEdGraphSelectNode.h"


FDialogueGraphDetailViewModel::FDialogueGraphDetailViewModel(const TSharedPtr<IPropertyHandle>& PropertyHandle)
{
    if ( !PropertyHandle.IsValid() || !PropertyHandle->IsValidHandle() )
    {
        checkf(false, TEXT("PropertyHandle is invalid in %s"), TEXT(__FUNCTION__));
        return;
    }

    AssetHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(UDialogueGraph, AssetUserData));
    // AssetHandle       = PropertyHandle->GetChildHandle(FName("DialogueEdGraph"));
    DescriptionHandle = PropertyHandle->GetChildHandle(FName("Description"));
    MemoHandle        = PropertyHandle->GetChildHandle(FName("Memo"));
}

void FDialogueGraphDetailViewModel::HideProperties(IDetailLayoutBuilder& Builder) const
{
    if ( !AssetHandle.IsValid() || !DescriptionHandle.IsValid() || !MemoHandle.IsValid() )
    {
        checkf(false, TEXT("One or more handles are invalid in %s"), TEXT(__FUNCTION__));
        return;
    }

    Builder.HideProperty(AssetHandle);
    Builder.HideProperty(DescriptionHandle);
    Builder.HideProperty(MemoHandle);
}

FString FDialogueGraphDetailViewModel::GetDescription() const
{
    if ( !DescriptionHandle.IsValid() || !DescriptionHandle->IsValidHandle() )
    {
        checkf(false, TEXT("DescriptionHandle is invalid in %s"), TEXT(__FUNCTION__));
        return "";
    }

    FString Result;
    DescriptionHandle->GetValueAsFormattedString(Result);
    return Result;
}

FString FDialogueGraphDetailViewModel::GetMemo() const
{
    if ( !MemoHandle.IsValid() || !MemoHandle->IsValidHandle() )
    {
        checkf(false, TEXT("MemoHandle is invalid in %s"), TEXT(__FUNCTION__));
        return "";
    }

    FString Result;
    MemoHandle->GetValueAsFormattedString(Result);
    return Result;
}

void FDialogueGraphDetailViewModel::SetDescription(const FString& InDesc) const
{
    if ( !DescriptionHandle.IsValid() || !DescriptionHandle->IsValidHandle() )
    {
        checkf(false, TEXT("DescriptionHandle is invalid in %s"), TEXT(__FUNCTION__));
        return;
    }
    DescriptionHandle->SetValueFromFormattedString(InDesc);
}

void FDialogueGraphDetailViewModel::SetMemo(const FString& InMemo) const
{
    if ( !MemoHandle.IsValid() || !MemoHandle->IsValidHandle() )
    {
        checkf(false, TEXT("MemoHandle is invalid in %s"), TEXT(__FUNCTION__));
        return;
    }

    MemoHandle->SetValueFromFormattedString(InMemo);
}

/// <summary>
/// 그래프의 노드 개수를 반환
/// </summary>
/// <returns>포함된 그래프의 노드 개수(만약 그래프가 없다면 0을 반환)</returns>
uint32 FDialogueGraphDetailViewModel::CountNode() const
{
    if ( !AssetHandle.IsValid() || !AssetHandle->IsValidHandle() )
    {
        checkf(false, TEXT("AssetHandle is invalid in %s"), TEXT(__FUNCTION__));
        return 0;
    }

    if ( UObject* Object; AssetHandle->GetValue(Object) == FPropertyAccess::Success )
    {
        if ( const auto AssetData = Cast<UDialogueEdGraphAssetData>(Object) )
        {
            if ( const UDialogueEdGraph* DialogueGraph = AssetData->DialogueEdGraph; DialogueGraph )
            {
                return DialogueGraph->Nodes.Num();
            }
        }
    }
    return 0;
}

uint32 FDialogueGraphDetailViewModel::CountSelectNode() const
{
    if ( !AssetHandle.IsValid() || !AssetHandle->IsValidHandle() )
    {
        checkf(false, TEXT("AssetHandle is invalid in %s"), TEXT(__FUNCTION__));
        return 0;
    }

    if ( UObject* Object; AssetHandle->GetValue(Object) == FPropertyAccess::Success )
    {
        if ( const auto AssetData = Cast<UDialogueEdGraphAssetData>(Object) )
        {
            if ( const UDialogueEdGraph* DialogueGraph = AssetData->DialogueEdGraph; DialogueGraph )
            {
                return Algo::CountIf(DialogueGraph->Nodes, [](const UEdGraphNode* Node)
                {
                    return Node->IsA<UDialogueEdGraphSelectNode>();
                });
            }
        }
    }
    return 0;
}

/// <summary>
/// 그래프의 대화 노드 개수를 반환 
/// </summary>
/// <returns> DialogueGraph의 DialogueLineNode 개수</returns>
uint32 FDialogueGraphDetailViewModel::CountLineNode() const
{
    if ( !AssetHandle.IsValid() || !AssetHandle->IsValidHandle() )
    {
        checkf(false, TEXT("AssetHandle is invalid in %s"), TEXT(__FUNCTION__));
        return 0;
    }

    if ( UObject* Object; AssetHandle->GetValue(Object) == FPropertyAccess::Success )
    {
        if ( const auto AssetData = Cast<UDialogueEdGraphAssetData>(Object) )
        {
            if ( const UDialogueEdGraph* DialogueGraph = AssetData->DialogueEdGraph; DialogueGraph )
            {
                return Algo::CountIf(DialogueGraph->Nodes, [](const UEdGraphNode* Node)
                {
                    return Node->IsA<UDialogueEdGraphDialogueLineNode>();
                });
            }
        }
    }
    return 0;
}

void FDialogueGraphDetailViewModel::SetOnAssetChanged(const FSimpleDelegate& OnAssetChanged) const
{
    if ( !AssetHandle.IsValid() || !AssetHandle->IsValidHandle() )
    {
        checkf(false, TEXT("AssetHandle is invalid in %s"), TEXT(__FUNCTION__));
        return;
    }

    AssetHandle->SetOnPropertyValueChanged(OnAssetChanged);
}

void FDialogueGraphDetailViewModel::SetOnDescriptionChanged(const FSimpleDelegate& OnDescriptionChanged) const
{
    if ( !DescriptionHandle.IsValid() || !DescriptionHandle->IsValidHandle() )
    {
        checkf(false, TEXT("DescriptionHandle is invalid in %s"), TEXT(__FUNCTION__));
        return;
    }

    DescriptionHandle->SetOnPropertyValueChanged(OnDescriptionChanged);
}

void FDialogueGraphDetailViewModel::SetOnMemoChanged(const FSimpleDelegate& OnMemoChanged) const
{
    if ( !MemoHandle.IsValid() || !MemoHandle->IsValidHandle() )
    {
        checkf(false, TEXT("MemoHandle is invalid in %s"), TEXT(__FUNCTION__));
        return;
    }

    MemoHandle->SetOnPropertyValueChanged(OnMemoChanged);
}
