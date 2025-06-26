// Fill out your copyright notice in the Description page of Project Settings.

#include "Graph/Slate/Node/SDialogueEdGraphSelectNode.h"

#include "GraphEditorSettings.h"
#include "SGraphPin.h"
#include "Graph/Node/DialogueEdGraphSelectNode.h"

void SDialogueEdGraphSelectNode::Construct(const FArguments& InArgs,
                                           UEdGraphNode* SelectNode)
{
    GraphNode = Cast<UEdGraphNode>(SelectNode);
    this->SetCursor(EMouseCursor::CardinalCross);
    UpdateGraphNode();
}

void SDialogueEdGraphSelectNode::CreatePinWidgets()
{
    for ( UEdGraphPin* Pin : GraphNode->Pins )
    {
        TSharedPtr<SGraphPin> PinWidget = SNew(SGraphPin, Pin);
        PinWidget->SetShowLabel(Pin->Direction == EGPD_Output);
        AddPin(PinWidget.ToSharedRef());
    }
}

FReply SDialogueEdGraphSelectNode::OnAddPin()
{
    UDialogueEdGraphSelectNode* SelectNode = CastChecked<UDialogueEdGraphSelectNode>(GraphNode);
    // 만약 사용하지 않는 핀이 있다면 새로운 핀을 생성하지 않습니다.
    SelectNode->AddOutputPin();
    UpdateGraphNode();
    return FReply::Handled();
}

TSharedRef<SWidget> SDialogueEdGraphSelectNode::CreateNodeContentArea()
{
    const TSharedRef<SWidget> AddPinButton = AddPinButtonContent(
                                                                 NSLOCTEXT("DialogueSelectNode", "DialogueSelectAddPinButton", "Add pin"),
                                                                 NSLOCTEXT("DialogueSelectNode", "DialogueSelectAddPinButton_ToolTip", "Add new pin"));

    FMargin AddPinPadding = Settings->GetOutputPinPadding();
    AddPinPadding.Top += 6.0f;

    /// 노드 콘텐츠 영역 (좌우 핀 포함) 생성
    return SNew(SHorizontalBox)
           /// 왼쪽(입력) 핀 영역
           + SHorizontalBox::Slot()
           .AutoWidth()
           [
               SAssignNew(LeftNodeBox, SVerticalBox)
           ]

           /// 노드 중앙 콘텐츠 영역
           + SHorizontalBox::Slot()
             .FillWidth(1.f)
             // .AutoWidth()
             .HAlign(HAlign_Center)
             .VAlign(VAlign_Center)
           [
               SNew(STextBlock)
               // .Text(FText::FromString(SelectNode->GetName()))
               .ColorAndOpacity(FLinearColor::White)
           ]

           /// 오른쪽(출력) 핀 영역
           + SHorizontalBox::Slot()
           .AutoWidth()
           [
               SNew(SVerticalBox)
               + SVerticalBox::Slot()
               .AutoHeight()
               [
                   SAssignNew(RightNodeBox, SVerticalBox)
               ]
               + SVerticalBox::Slot()
               .AutoHeight()
               [
                   SNew(SBox)
                   .MinDesiredHeight(25)
                   .MinDesiredWidth(25)
                   .Padding(AddPinPadding)
                   [
                       AddPinButton
                   ]
               ]
           ];
}

const FSlateBrush* SDialogueEdGraphSelectNode::GetNodeBodyBrush() const
{
    return FAppStyle::GetBrush("Graph.Node.Body");
}
