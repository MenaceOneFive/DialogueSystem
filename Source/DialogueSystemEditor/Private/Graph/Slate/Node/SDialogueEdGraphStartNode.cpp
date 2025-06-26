// Fill out your copyright notice in the Description page of Project Settings.

#include "Graph/Slate/Node/SDialogueEdGraphStartNode.h"

#include "SGraphPin.h"
#include "Graph/Node/DialogueEdGraphStartNode.h"

void SDialogueEdGraphStartNode::Construct(const FArguments& InArgs,
                                          UEdGraphNode* StartNode)
{
    GraphNode = Cast<UEdGraphNode>(StartNode);
    this->SetCursor(EMouseCursor::CardinalCross);
    UpdateGraphNode();
}


void SDialogueEdGraphStartNode::CreatePinWidgets()
{
    for ( UEdGraphPin* Pin : GraphNode->Pins )
    {
        TSharedPtr<SGraphPin> PinWidget = SNew(SGraphPin, Pin);
        PinWidget->SetShowLabel(Pin->bHidden);
        AddPin(PinWidget.ToSharedRef());
    }
}

TSharedRef<SWidget> SDialogueEdGraphStartNode::CreateNodeContentArea()
{
    UDialogueEdGraphStartNode* StartNode = CastChecked<UDialogueEdGraphStartNode>(GraphNode);
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
           .MinWidth(100.f)
           .FillWidth(1.f)
           .HAlign(HAlign_Center)
           .VAlign(VAlign_Center)
           [
               SNew(STextBlock)
               // .Text(FText::FromString(StartNode->GetName()))
               .ColorAndOpacity(FLinearColor::White)
           ]

           /// 오른쪽(출력) 핀 영역
           + SHorizontalBox::Slot()
           .AutoWidth()
           [
               SAssignNew(RightNodeBox, SVerticalBox)
           ];
}

const FSlateBrush* SDialogueEdGraphStartNode::GetNodeBodyBrush() const
{
    return FAppStyle::GetBrush("Graph.Node.Body");
}
