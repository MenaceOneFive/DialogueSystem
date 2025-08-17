// Fill out your copyright notice in the Description page of Project Settings.

#include "Graph/Slate/Node/SDialogueEdGraphLineNode.h"

#include "SGraphPin.h"
#include "Character/DialogueCharacterAsset.h"
#include "Graph/Node/DialogueEdGraphDialogueLineNode.h"
#include "Widgets/SCanvas.h"
#include "Widgets/Text/SMultiLineEditableText.h"

void SDialogueEdGraphLineNode::Construct(const FArguments& InArgs,
                                         UEdGraphNode* LineNode)
{
    GraphNode = Cast<UEdGraphNode>(LineNode);
    this->SetCursor(EMouseCursor::CardinalCross);
    if ( const auto Node = Cast<UDialogueEdGraphDialogueLineNode>(LineNode) )
    {
        DialogueLineNode = Node;
    }
    UpdateGraphNode();
}

const FSlateBrush* SDialogueEdGraphLineNode::GetNodeBodyBrush() const
{
    return SGraphNode::GetNodeBodyBrush();
}

void SDialogueEdGraphLineNode::CreatePinWidgets()
{
    for ( UEdGraphPin* Pin : GraphNode->Pins )
    {
        TSharedPtr<SGraphPin> PinWidget = SNew(SGraphPin, Pin);
        PinWidget->SetShowLabel(Pin->bHidden);
        AddPin(PinWidget.ToSharedRef());
    }
}

void SDialogueEdGraphLineNode::UpdateGraphNode()
{
    SGraphNode::UpdateGraphNode();
    if ( IsValid(DialogueLineNode) && DialogueLineTextBox.IsValid() )
    {
        DialogueLineTextBox->SetText(FText::FromString(DialogueLineNode->DialogueLine));
    }
}

void SDialogueEdGraphLineNode::MakeImageBrush()
{
    if ( DialogueLineNode && DialogueLineNode->DialogueCharacterAsset )
    {
        SlateBrush = MakeShared<FSlateBrush>();
        SlateBrush->SetResourceObject(DialogueLineNode->DialogueCharacterAsset->CharacterProfileImage);
        SlateBrush->SetImageSize({65.f, 65.f});
        constexpr float CornerRadius             = 20.f;
        SlateBrush->DrawAs                       = ESlateBrushDrawType::Type::RoundedBox;
        SlateBrush->OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
        SlateBrush->OutlineSettings.CornerRadii  = FVector4(CornerRadius, CornerRadius, CornerRadius, CornerRadius);
    }
}

TSharedRef<SWidget> SDialogueEdGraphLineNode::CreateTitleWidget(const TSharedPtr<SNodeTitle> NodeTitle)
{
    if ( DialogueLineNode && DialogueLineNode->DialogueCharacterAsset )
    {
        if ( !SlateBrush )
        {
            MakeImageBrush();
        }
        return SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
            .FillWidth(1.f)
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Center)
            [
                SNew(SCanvas)
                + SCanvas::Slot()
                .Position(FVector2d{-40.f, -60.f})
                .Size(FVector2d{SlateBrush->GetImageSize().X, SlateBrush->GetImageSize().Y})
                [
                    SNew(SImage)
                    .Image(SlateBrush.Get())
                ]
            ]
            + SHorizontalBox::Slot()
            .FillWidth(10.f)
            .HAlign(HAlign_Center)
            .VAlign(VAlign_Center)
            [
                SGraphNode::CreateTitleWidget(NodeTitle)
            ];
    }
    return SGraphNode::CreateTitleWidget(NodeTitle);
}

TSharedRef<SWidget> SDialogueEdGraphLineNode::CreateNodeContentArea()
{
    // 노드 콘텐츠 영역 (좌우 핀 포함) 생성
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
               SAssignNew(DialogueLineTextBox, SMultiLineEditableText)
               .AllowMultiLine(true)
               .IsReadOnly(true)
               .WrapTextAt(250.f)
               .Text(FText::FromString(IsValid(DialogueLineNode) ? DialogueLineNode->DialogueLine : ""))
           ]


           /// 오른쪽(출력) 핀 영역
           + SHorizontalBox::Slot()
           .AutoWidth()
           [
               SAssignNew(RightNodeBox, SVerticalBox)
           ];
}
