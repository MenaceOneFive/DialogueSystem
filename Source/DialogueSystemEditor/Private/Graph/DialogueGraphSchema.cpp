// Fill out your copyright notice in the Description page of Project Settings.

#include "Graph/DialogueGraphSchema.h"

#include "ToolMenus.h"
#include "Framework/Commands/GenericCommands.h"
#include "Graph/DialogueGraphEditorCommands.h"
#include "Graph/Node/DialogueEdGraphDialogueLineNode.h"
#include "Graph/Node/DialogueEdGraphEndNode.h"
#include "Graph/Node/DialogueEdGraphNode.h"
#include "Graph/Node/DialogueEdGraphSelectNode.h"

#define LOCTEXT_NAMESPACE "DialogueGraph"

/// <summary>
/// 마우스 우클릭시 호출되는 ContextAction
/// </summary>
#pragma region SchemaContextMenuActions

UEdGraphNode* FDialogueGraphSchema_SpawnNode::PerformAction(UEdGraph* ParentGraph,
                                                            UEdGraphPin* FromPin,
                                                            const FVector2D Location,
                                                            bool bSelectNewNode)
{
    const FScopedTransaction Transaction(LOCTEXT("AddDialogueNode", "Add a new DialogueNode to graph"));

    UDialogueEdGraphNode* NewNode = NewObject<UDialogueEdGraphNode>(ParentGraph,
                                                                    SpawnableNodeClass,
                                                                    NAME_None,
                                                                    RF_Transactional);
    ParentGraph->Modify();
    ParentGraph->AddNode(NewNode, true);
    NewNode->SetNodePosition(static_cast<int32>(Location.X),
                             static_cast<int32>(Location.Y));
    NewNode->AllocateDefaultPins();
    if ( !FromPin ) // 핀을 드래그해서 호출한 경우가 아니라면 초기화는 이 정도로 완료하고 끝
    {
        ParentGraph->NotifyGraphChanged();
        return NewNode;
    }
    FromPin->Modify();
    NewNode->AutowireNewNode(FromPin);
    ParentGraph->NotifyGraphChanged();

    return NewNode;
}

void UDialogueGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
    // FGraphContextMenuBuilder의 FGraphActionMenuBuilder영역에 드래그 시작점에 대한 참조가 들어있다.
    // 핀 Type에 따른 Context Action

    // 종료노드
    const TSharedRef<FDialogueGraphSchema_SpawnNode> SpawnEndDialogueAction =
            MakeShareable(new FDialogueGraphSchema_SpawnNode(UDialogueEdGraphEndNode::StaticClass(),
                                                             FText::FromString("End Node"),
                                                             FText::FromString("End Node2"),
                                                             FText::FromString("End")));
    ContextMenuBuilder.AddAction(SpawnEndDialogueAction);

    // // 장면 노드
    // const TSharedRef<FDialogueGraphSchema_SpawnNode> SpawnSceneDialogueAction =
    //         MakeShareable(new FDialogueGraphSchema_SpawnNode(UDialogueEdGraphSceneNode::StaticClass(),
    //                                                          FText::FromString("Scene Node"),
    //                                                          FText::FromString("Scene Node2"),
    //                                                          FText::FromString("Scene")));
    // ContextMenuBuilder.AddAction(SpawnSceneDialogueAction);

    // 대사 노드
    const TSharedRef<FDialogueGraphSchema_SpawnNode> DialogueLineNodeAction =
            MakeShareable(new FDialogueGraphSchema_SpawnNode(UDialogueEdGraphDialogueLineNode::StaticClass(),
                                                             FText::FromString("Dialogue Line Node"),
                                                             FText::FromString("Dialogue Line Node"),
                                                             FText::FromString("Line Node")));
    ContextMenuBuilder.AddAction(DialogueLineNodeAction);

    // 선택 노드
    const TSharedRef<FDialogueGraphSchema_SpawnNode> SpawnSelectDialogueAction =
            MakeShareable(new FDialogueGraphSchema_SpawnNode(UDialogueEdGraphSelectNode::StaticClass(),
                                                             FText::FromString("Select Node"),
                                                             FText::FromString("Select Node2"),
                                                             FText::FromString("Select")));
    ContextMenuBuilder.AddAction(SpawnSelectDialogueAction);

    Super::GetGraphContextActions(ContextMenuBuilder);
}

/// <summary>
/// 그래프에서 컨텍스트 메뉴를 생성해야 할 때 호출되는 메서드
/// </summary>
/// <param name="Menu">컨텍스트 메뉴</param>
/// <param name="Context">컨텍스트</param>
void UDialogueGraphSchema::GetContextMenuActions(UToolMenu* Menu,
                                                 UGraphNodeContextMenuContext* Context) const
{
    // Context에 선택한 그래프, 노드, 핀에 대한 참조가 들어 있다.
    checkf(Menu, TEXT("Menu can't be empty"));
    Super::GetContextMenuActions(Menu, Context);

    const FGenericCommands& GenericCommands = FGenericCommands::Get();

    // Node-specific actions
    if ( Context->Node )
    {
        FToolMenuSection& Section = Menu->FindOrAddSection(FName("NodeActions"));
        Section.AddMenuEntry(GenericCommands.Cut);
        Section.AddMenuEntry(GenericCommands.Copy);
        Section.AddMenuEntry(GenericCommands.Duplicate);

        // Custom delete commands
        const FToolMenuEntry DeleteConnectionEntry = FToolMenuEntry::InitMenuEntry(
                                                                                   FDialogueGraphEditorCommands::Get().DeleteAllNodeConnection,
                                                                                   NSLOCTEXT("DialogueGraphEditor", "DeleteConnection", "Delete Connection"),
                                                                                   NSLOCTEXT("DialogueGraphEditor", "DeleteConnectionTooltip", "Delete the selected connection between nodes")
                                                                                  );
        Section.AddEntry(DeleteConnectionEntry);

        const FToolMenuEntry DeleteNodeEntry = FToolMenuEntry::InitMenuEntry(
                                                                             FDialogueGraphEditorCommands::Get().DeleteSelectedNode,
                                                                             NSLOCTEXT("DialogueGraphEditor", "DeleteNode", "Delete Node"),
                                                                             NSLOCTEXT("DialogueGraphEditor", "DeleteNodeTooltip", "Delete the selected node")
                                                                            );
        Section.AddEntry(DeleteNodeEntry);
    }

    // General graph actions
    {
        FToolMenuEntry::InitMenuEntry(FGenericCommands::Get().Paste);
        FToolMenuSection& Section = Menu->FindOrAddSection(FName("GraphActions"));
        Section.AddMenuEntry(GenericCommands.Paste);
    }
}

#pragma endregion

/// <summary>
/// 핀 연결과 관련된 메서드들
/// </summary>
#pragma region PinConnection

const FPinConnectionResponse UDialogueGraphSchema::CanCreateConnection(const UEdGraphPin* A,
                                                                       const UEdGraphPin* B) const
{
    // 정책 목록을 순회 -> 모든 정책 검사를 통과해야 연결이 허용됨
    // 정책은 .h파일의 ConnectionResponse의 InitializerList에 추가하면 됨
    // TArray<TSharedPtr<FPinConnectionPolicy>> UDialogueGraphSchema::ConnectionPolicies

    FPinConnectionResponse ConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
    for ( TSharedPtr PinConnectionPolicy : ConnectionPolicies )
    {
        if ( PinConnectionPolicy.IsValid() )
        {
            if ( auto Response = PinConnectionPolicy->ValidateConnection(A, B);
                Response.Response != CONNECT_RESPONSE_MAKE )
            {
                if ( Response.Response == CONNECT_RESPONSE_DISALLOW )
                {
                    return Response;
                }
                ConnectionResponse = Response;
            }
        }
    }
    return ConnectionResponse;
}

FConnectionDrawingPolicy* UDialogueGraphSchema::CreateConnectionDrawingPolicy(int32 InBackLayerID,
                                                                              int32 InFrontLayerID,
                                                                              float InZoomFactor,
                                                                              const FSlateRect& InClippingRect,
                                                                              FSlateWindowElementList& InDrawElements,
                                                                              UEdGraph* InGraphObj) const
{
    // 연결선 그리기 정책 생성
    return nullptr;
}

#pragma endregion

/// <summary>
/// 핀 연결 해제와 관련된 메서드들
/// </summary>
#pragma region BreakPin

void UDialogueGraphSchema::BreakNodeLinks(UEdGraphNode& TargetNode) const
{
    // 노드 링크 해제 로직
    Super::BreakNodeLinks(TargetNode);
}

void UDialogueGraphSchema::BreakPinLinks(UEdGraphPin& TargetPin,
                                         bool bSendsNodeNotification) const
{
    // 핀 링크 해제 로직
    Super::BreakPinLinks(TargetPin, bSendsNodeNotification);
}

#pragma endregion

FLinearColor UDialogueGraphSchema::GetPinTypeColor(const FEdGraphPinType& PinType) const
{
    // 핀 타입별 색상 정의
    return FLinearColor::White;
}

#undef LOCTEXT_NAMESPACE
