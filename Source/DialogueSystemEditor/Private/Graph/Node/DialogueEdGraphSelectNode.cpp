#include "Graph/Node/DialogueEdGraphSelectNode.h"
#include "EdGraph/EdGraph.h"
#include "Graph/Node/DialogueSelectionNode.h"
#include "Visitor/AbstractDialogueEdGraphVisitor.h"

#define LOCTEXT_NAMESPACE "FDialogueGraphEditor"

UDialogueEdGraphSelectNode::UDialogueEdGraphSelectNode()
{
    DialogueSetting.EFocusMode        = EFocusMode::UI;
    DialogueSetting.bShouldShowCursor = true;
}

void UDialogueEdGraphSelectNode::CopyTo(const TObjectPtr<UDialogueSelectionNode>& SelectionNode) const
{
    SelectionNode->SetWhenSelectThisNodeFunctionName(GetWhenSelectThisNodeFunctionName());
    SelectionNode->SetCanSelectThisNodeFunctionName(GetCanSelectThisNodeFunctionName());
    auto Name = GetSelectionName();
    SelectionNode->SetSelectionName(Name);
    SelectionNode->SetDialogueSetting(DialogueSetting);
}

FText UDialogueEdGraphSelectNode::GetDefaultSelectionName() const
{
    return NSLOCTEXT("DialogueEditorNode", "SelectionNode", "선택 노드");
}

FText UDialogueEdGraphSelectNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    return FText::FromString(TEXT("선택지"));
}

void UDialogueEdGraphSelectNode::AllocateDefaultPins()
{
    Super::AllocateDefaultPins();
    UEdGraphPin* PrevNodePin  = CreatePin(EGPD_Input, PC_Exec, "PrevNode");
    PrevNodePin->bHidden      = false;
    UEdGraphPin* NextNodePin1 = CreatePin(EGPD_Output, PC_Exec, FName(FString::Format(TEXT("선택지 {0}"), {++PinCounter})));
    NextNodePin1->bHidden     = false;
    UEdGraphPin* NextNodePin2 = CreatePin(EGPD_Output, PC_Exec, FName(FString::Format(TEXT("선택지 {0}"), {++PinCounter})));
    NextNodePin2->bHidden     = false;
    UEdGraphPin* NextNodePin3 = CreatePin(EGPD_Output, PC_Exec, FName(FString::Format(TEXT("선택지 {0}"), {++PinCounter})));
    NextNodePin3->bHidden     = false;
}

FText UDialogueEdGraphSelectNode::GetTooltipText() const
{
    return FText::FromString(TEXT("플레이어에게 주는 선택지"));
}

void UDialogueEdGraphSelectNode::AddOutputPin()
{
    for (const UEdGraphPin* Pin : Pins)
    {
        // 사용중이지 않은 핀이 있는 경우 새로 핀을 추가하지 않음
        if (!Pin->IsPendingKill() && Pin->LinkedTo.IsEmpty())
        {
            return;
        }
    }
    const FScopedTransaction Transaction(LOCTEXT("AddPinToNode", "Adding Pin to Selection Node"));
    UEdGraphPin* NextNodePin = CreatePin(EGPD_Output, PC_Exec, FName(FString::Format(TEXT("선택지 {0}"), {++PinCounter})));
    NextNodePin->bHidden     = false;
}

void UDialogueEdGraphSelectNode::Accept(FAbstractDialogueEdGraphVisitor* Visitor)
{
    Visitor->VisitSelectNode(this);
}

TArray<TObjectPtr<const UDialogueEdGraphNode>> UDialogueEdGraphSelectNode::GetNextNodes() const
{
    return ::GetNextNodes<ThisClass>(this);
}

TArray<TObjectPtr<const UDialogueEdGraphNode>> UDialogueEdGraphSelectNode::GetPrevNodes() const
{
    return ::GetPrevNodes<ThisClass>(this);
}

#undef LOCTEXT_NAMESPACE
