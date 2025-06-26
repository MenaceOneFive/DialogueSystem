#pragma once

#include "UObject/Object.h"
#include "Graph/Node/DialogueNodeInterface.h"
#include "DialogueSelectionItem.generated.h"

class UDialogueGraphDirector;
class IRuntimeDialogueGraphVisitor;
class UDialogueGraphNode;

UCLASS(Blueprintable)
class DIALOGUESYSTEMRUNTIME_API UDialogueSelectionItem : public UObject
{
    GENERATED_BODY()

public:
    // UI에서 표시될 텍스트
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SelectionName;

    // 이 선택지를 선택할 수 있는지 확인하는 메서드
    UFUNCTION(BlueprintCallable)
    bool CanSelectThisItem();

    // 이 선택지를 선택해서 다음 노드를 방문
    UFUNCTION(BlueprintCallable)
    void SelectThisItem();

    // 선택지 선택 이벤트를 초기화
    UFUNCTION(BlueprintCallable)
    void InitializeForExecution(UDialogueGraphDirector* Director,
                                TScriptInterface<IRuntimeDialogueGraphVisitor> Visitor);

#if WITH_EDITOR
    void SetNextNodeToVisit(UDialogueGraphNode* NextNode);
#endif

private:
    UPROPERTY()
    TObjectPtr<UDialogueGraphNode> NextNodeToVisit;

    FSimpleDelegate VisitNextNodeDelegate;

    // 이 항목이 선택 가능한지 확인할 때 사용
    FCanSelectThisItem CanSelectThisItemDelegate;
};
