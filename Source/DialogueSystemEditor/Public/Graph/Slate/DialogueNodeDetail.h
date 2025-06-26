#pragma once

#include "IDetailCustomization.h"
#include "Graph/DialogueBlueprintUtils.h"

class SComboButton;
class ITableRow;
class STableViewBase;
class UDialogueEdGraphNode;


class FDialogueNodeDetail : public IDetailCustomization
{
public: // 생성자
    static TSharedRef<IDetailCustomization> MakeInstance();

public: // IDetailCustomization
    FReply OnOpenFunctionGraph(const FName& FunctionName) const;
    FReply OnOpenCanSelectFunction() const;
    FReply OnOpenWhenSelectFunction() const;
    virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

private: // Custom Widgets
    /// <summary>
    /// SListView의 각 행에 대한 위젯을 생성합니다. (공통 사용)
    /// </summary>
    TSharedRef<ITableRow> OnGenerateRowForNameListView(TSharedPtr<FName> InItem,
                                                       const TSharedRef<STableViewBase>& OwnerTable) const;

    // --- CanSelectThisNode용 델리게이트 --- 
    TSharedRef<SWidget> OnGetMenuContent_CanSelect();
    void OnSelectionChanged_CanSelect(TSharedPtr<FName> NewSelection,
                                      ESelectInfo::Type SelectInfo) const;
    FText GetCurrentFunctionName_CanSelect() const;

    // --- WhenSelectThisNode용 델리게이트 ---
    TSharedRef<SWidget> OnGetMenuContent_WhenSelect();
    void OnSelectionChanged_WhenSelect(TSharedPtr<FName> NewSelection,
                                       ESelectInfo::Type SelectInfo) const;
    FText GetCurrentFunctionName_WhenSelect() const;

private: // Helper Functions
    /// <summary>
    /// 편집 중인 노드와 관련된 블루프린트의 생성된 클래스를 가져옵니다.
    /// <br>
    /// 중첩된 if 문을 줄이고 코드를 단순화하는 데 사용됩니다.
    /// </summary>
    /// <returns>성공 시 UClass에 대한 포인터, 실패 시 nullptr</returns>
    const UClass* GetBlueprintGeneratedClass() const;
    UEdGraph* CreateNewFunction_WhenSelect(FName NewFunctionName) const;
    UEdGraph* CreateNewFunction_CanSelect(FName NewFunctionName) const;

private:
    // 커스터마이징 중인 그래프 노드에 대한 약한 포인터.
    TWeakObjectPtr<UDialogueEdGraphNode> EdGraphNode;

    // 각 콤보박스에 표시될 함수 이름 목록.
    TArray<TSharedPtr<FName>> CanSelectFunctionNames;
    TArray<TSharedPtr<FName>> WhenSelectFunctionNames;

    // 각 콤보 버튼에 대한 참조.
    TSharedPtr<SComboButton> CanSelectComboButton;
    TSharedPtr<SComboButton> WhenSelectComboButton;

    FString MakeFunctionName(EFunctionType FunctionType,
                             const UDialogueEdGraphNode* Node) const;

    FName CreateNewFunction = FName("Create New Function");
};
