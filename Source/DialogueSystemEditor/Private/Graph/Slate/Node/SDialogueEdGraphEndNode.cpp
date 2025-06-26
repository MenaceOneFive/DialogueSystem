// Fill out your copyright notice in the Description page of Project Settings.

#include "Graph/Slate/Node/SDialogueEdGraphEndNode.h"

#include "SGraphPin.h"

void SDialogueEdGraphEndNode::Construct(const FArguments& InArgs,
                                        UEdGraphNode* EndNode)
{
    GraphNode = Cast<UEdGraphNode>(EndNode);
    this->SetCursor(EMouseCursor::CardinalCross);
    UpdateGraphNode();
}

void SDialogueEdGraphEndNode::CreatePinWidgets()
{
    for ( UEdGraphPin* Pin : GraphNode->Pins )
    {
        TSharedPtr<SGraphPin> PinWidget = SNew(SGraphPin, Pin);
        PinWidget->SetShowLabel(Pin->bHidden);
        AddPin(PinWidget.ToSharedRef());
    }
}

TSharedRef<SWidget> SDialogueEdGraphEndNode::CreateNodeContentArea()
{
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
           .MinWidth(100.f)
           .HAlign(HAlign_Center)
           .VAlign(VAlign_Center)
           [
               SNew(STextBlock)
               // .Text(FText::FromString(EndNode->GetName()))
               .ColorAndOpacity(FLinearColor::White)
           ]

           /// 오른쪽(출력) 핀 영역
           + SHorizontalBox::Slot()
           .AutoWidth()
           [
               SAssignNew(RightNodeBox, SVerticalBox)
           ];
}

const FSlateBrush* SDialogueEdGraphEndNode::GetNodeBodyBrush() const
{
    return FAppStyle::GetBrush("Graph.Node.Body");
}
