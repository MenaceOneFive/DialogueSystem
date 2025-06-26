// Fill out your copyright notice in the Description page of Project Settings.

#include "Graph/Slate/Node/SDialogueEdGraphSceneNode.h"

#include "SGraphPin.h"
#include "Graph/Node/DialogueEdGraphSceneNode.h"

void SDialogueEdGraphSceneNode::Construct(const FArguments& InArgs,
                                          UEdGraphNode* SceneNode)
{
    GraphNode = Cast<UEdGraphNode>(SceneNode);
    this->SetCursor(EMouseCursor::CardinalCross);
    UpdateGraphNode();
}


void SDialogueEdGraphSceneNode::CreatePinWidgets()
{
    for ( UEdGraphPin* Pin : GraphNode->Pins )
    {
        TSharedPtr<SGraphPin> PinWidget = SNew(SGraphPin, Pin);
        PinWidget->SetShowLabel(Pin->bHidden);
        AddPin(PinWidget.ToSharedRef());
    }
}

TSharedRef<SWidget> SDialogueEdGraphSceneNode::CreateNodeContentArea()
{
    UDialogueEdGraphSceneNode* SceneNode = CastChecked<UDialogueEdGraphSceneNode>(GraphNode);
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
               // .Text(FText::FromString(SceneNode->GetName()))
               .ColorAndOpacity(FLinearColor::White)
           ]

           /// 오른쪽(출력) 핀 영역
           + SHorizontalBox::Slot()
           .AutoWidth()
           [
               SAssignNew(RightNodeBox, SVerticalBox)
           ];
}

const FSlateBrush* SDialogueEdGraphSceneNode::GetNodeBodyBrush() const
{
    return FAppStyle::GetBrush("Graph.Node.Body");
}
