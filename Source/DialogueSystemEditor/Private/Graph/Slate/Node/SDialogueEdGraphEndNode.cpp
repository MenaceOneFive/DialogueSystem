// Fill out your copyright notice in the Description page of Project Settings.

#include "Graph/Slate/Node/SDialogueEdGraphEndNode.h"

#include "SGraphPin.h"
#include "Graph/DialogueEdGraphNodes.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"

void SDialogueEdGraphEndNode::Construct(const FArguments& InArgs, UEdGraphNode* EndNode)
{
    GraphNode = Cast<UEdGraphNode>(EndNode);
    this->SetCursor(EMouseCursor::CardinalCross);
    UpdateGraphNode();
}

void SDialogueEdGraphEndNode::UpdateGraphNode()
{
    /// 핀 초기화
    InputPins.Empty();
    OutputPins.Empty();

    /// 노드 본문 레이아웃 생성
    TSharedPtr<SVerticalBox> NodeBody = SNew(SVerticalBox);

    /// 기본 노드 본문 레이아웃 설정
    TSharedPtr<SNodeTitle> NodeTitle = SNew(SNodeTitle, GraphNode);

    /// 노드 내용 영역 생성
    this->GetOrAddSlot(ENodeZone::Center)
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Center)
    [
        SNew(SBorder)
        .BorderImage(FAppStyle::GetBrush("Graph.Node.Body"))
        .BorderBackgroundColor(this->GetNodeBodyColor())
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Fill)
        .Padding(5.0f)
        [
            SNew(SVerticalBox)

            /// 헤더 영역
            + SVerticalBox::Slot()
            .AutoHeight()
            [
                SNew(SBorder)
                .BorderImage(FAppStyle::GetBrush("Graph.Node.TitleBackground"))
                .BorderBackgroundColor(this->GetNodeTitleColor())
                .HAlign(HAlign_Fill)
                .VAlign(VAlign_Center)
                .Padding(10.0f)
                [
                    SNew(SHorizontalBox)

                    /// 타이틀 텍스트
                    + SHorizontalBox::Slot()
                    .AutoWidth()
                    .VAlign(VAlign_Center)
                    [
                        SAssignNew(InlineEditableText, SInlineEditableTextBlock)
                        .Style(FAppStyle::Get(), "Graph.Node.NodeTitleInlineEditableText")
                        .Text(NodeTitle->GetHeadTitle())
                        .IsReadOnly(true)
                    ]
                ]
            ]

            /// 노드 콘텐츠 영역
            + SVerticalBox::Slot()
            .AutoHeight()
            [
                CreateNodeContentArea()
            ]
        ]
    ];

    /// 핀 위젯 생성
    CreatePinWidgets();
}

void SDialogueEdGraphEndNode::CreatePinWidgets()
{
    for (UEdGraphPin* Pin : GraphNode->Pins)
    {
        TSharedPtr<SGraphPin> PinWidget = SNew(SGraphPin, Pin);
        PinWidget->SetShowLabel(Pin->bHidden);
        AddPin(PinWidget.ToSharedRef());
    }
}

TSharedRef<SWidget> SDialogueEdGraphEndNode::CreateNodeContentArea()
{
    UDialogueEdGraphEndNode* EndNode = CastChecked<UDialogueEdGraphEndNode>(GraphNode);
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
