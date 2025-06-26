#pragma once

#include "CoreMinimal.h"
#include "DetailLayoutBuilder.h"
#include "Graph/DialogueGraphAssetFactory.h"


// ViewModel: 데이터와 View 연결, 바인딩 및 커맨드 처리
class FDialogueGraphDetailViewModel
{
public:
    explicit FDialogueGraphDetailViewModel(const TSharedPtr<IPropertyHandle>& PropertyHandle);;

    void HideProperties(IDetailLayoutBuilder& Builder) const;;

    FString GetDescription() const;
    FString GetMemo() const;

    void SetDescription(const FString& InDesc) const;
    void SetMemo(const FString& InMemo) const;

    void SetOnAssetChanged(const FSimpleDelegate& OnAssetChanged) const;
    void SetOnDescriptionChanged(const FSimpleDelegate& OnDescriptionChanged) const;
    void SetOnMemoChanged(const FSimpleDelegate& OnMemoChanged) const;

    uint32 CountNode() const;
    uint32 CountSelectNode() const;
    uint32 CountLineNode() const;

    TObjectPtr<UDialogueGraph> GetAsset() const
    {
        UObject* AssetToReturn;
        AssetHandle->GetValue(AssetToReturn);
        return {Cast<UDialogueGraph>(AssetToReturn->GetOuter())};
    }

protected:
    TSharedPtr<IPropertyHandle> AssetHandle;
    TSharedPtr<IPropertyHandle> DescriptionHandle;
    TSharedPtr<IPropertyHandle> MemoHandle;
};
