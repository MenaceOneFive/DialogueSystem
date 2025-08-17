#pragma once

#include "Graph/Node/DialogueNodeInterface.h"
#include "UObject/Object.h"
#include "DialogueSelectionItem.generated.h"

class UDialogueGraphDirector;
class IRuntimeDialogueGraphVisitor;
class UDialogueGraphNode;

UCLASS(Blueprintable)
class DIALOGUESYSTEMRUNTIME_API UDialogueSelectionItem : public UObject
{
    GENERATED_BODY()

public:
    // 이 선택지를 선택할 수 있는지 확인하는 메서드
    UFUNCTION(BlueprintCallable)
    bool CanSelectThisItem() const;

    // 이 선택지를 선택해서 다음 노드를 방문
    UFUNCTION(BlueprintCallable)
    void SelectThisItem() const;

    void Initialize(const FSimpleDelegate& WhenSelectThisNodeDelegate, bool CanSelectThisNode);

    UFUNCTION(BlueprintCallable)
    void SetSelectionName(const FText& InSelectionName);

private:
    // UI에서 표시될 텍스트
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
    FText SelectionName;

    FSimpleDelegate VisitNextNodeDelegate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
    bool bCanSelectThisItem;
};
