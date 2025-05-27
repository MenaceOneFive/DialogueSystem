// Fill out your copyright notice in the Description page of Project Settings.

#include "Graph/DialogueGraphSchema.h"

#include "Graph/DialogueEdGraphNodes.h"

#define LOCTEXT_NAMESPACE "DialogueGraph"

#pragma region SchemaContextMenuActions // 마우스 우클릭시 호출되는 ContextAction
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
    if (!FromPin) // 핀을 드래그해서 호출한 경우가 아니라면 초기화는 이 정도로 완료하고 끝
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

    // OwningNode, PinType
    TSharedRef<FDialogueGraphSchema_SpawnNode> SpawnEndDialogueAction =
            MakeShareable(new FDialogueGraphSchema_SpawnNode(UDialogueEdGraphEndNode::StaticClass(),
                                                             FText::FromString("End Node"),
                                                             FText::FromString("End Node2"),
                                                             FText::FromString("End")));
    ContextMenuBuilder.AddAction(SpawnEndDialogueAction);

    TSharedRef<FDialogueGraphSchema_SpawnNode> SpawnSceneDialogueAction =
            MakeShareable(new FDialogueGraphSchema_SpawnNode(UDialogueEdGraphSceneNode::StaticClass(),
                                                             FText::FromString("Scene Node"),
                                                             FText::FromString("Scene Node2"),
                                                             FText::FromString("Scene")));
    ContextMenuBuilder.AddAction(SpawnSceneDialogueAction);

    TSharedRef<FDialogueGraphSchema_SpawnNode> DialogueLineNodeAction =
            MakeShareable(new FDialogueGraphSchema_SpawnNode(UDialogueEdGraphDialogueLineNode::StaticClass(),
                                                             FText::FromString("Dialogue Line Node"),
                                                             FText::FromString("Dialogue Line Node"),
                                                             FText::FromString("Line Node")));
    ContextMenuBuilder.AddAction(DialogueLineNodeAction);

    TSharedRef<FDialogueGraphSchema_SpawnNode> SpawnSelectDialogueAction =
            MakeShareable(new FDialogueGraphSchema_SpawnNode(UDialogueEdGraphSelectNode::StaticClass(),
                                                             FText::FromString("Select Node"),
                                                             FText::FromString("Select Node2"),
                                                             FText::FromString("Select")));
    ContextMenuBuilder.AddAction(SpawnSelectDialogueAction);

    Super::GetGraphContextActions(ContextMenuBuilder);
}

void UDialogueGraphSchema::GetContextMenuActions(UToolMenu* Menu,
                                                 UGraphNodeContextMenuContext* Context) const
{
    // Context에 선택한 그래프, 노드, 핀에 대한 참조가 들어 있다.
    Super::GetContextMenuActions(Menu, Context);
}
#pragma endregion

#pragma region PinConnectionPolicies    // 핀간 연결이 적절한지 검사하는 정책들을 정의
FPinConnectionResponse FPinConnectionPolicy::ValidateConnection(const UEdGraphPin* A,
                                                                const UEdGraphPin* B)
{
    return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT(""));
}

FPinConnectionResponse FRecursivePinConnectionPolicy::ValidateConnection(const UEdGraphPin* A,
                                                                         const UEdGraphPin* B)
{
    const UDialogueEdGraphNode* A_Node = Cast<UDialogueEdGraphNode>(A->GetOwningNode());
    const UDialogueEdGraphNode* B_Node = Cast<UDialogueEdGraphNode>(B->GetOwningNode());

    checkf(A_Node && B_Node, TEXT("핀을 소유하고 있는 노드가 DialogueEdGraphNode가 아닙니다."));

    // A와 B가 같은 노드인 경우 재귀적인 연결이라 판단 : 노드에서 명시적으로 허용해야 연결을 허용한다.
    if (A_Node == B_Node)
    {
        if (A_Node->IsAllowRecursiveConnection())
        {
            return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT("재귀적 연결을 허용합니다."));
        }
        return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("재귀적 연결을 허용하지 않습니다."));
    }
    return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
}

FPinConnectionResponse FDirectionPinConnectionPolicy::ValidateConnection(const UEdGraphPin* A,
                                                                         const UEdGraphPin* B)
{
    // ReSharper disable once CppConditionalExpressionCanBeSimplified
    return A->Direction == B->Direction ?
               FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("입력 <-> 입력, 출력 <-> 출력 쌍은 연결을 허용하지 않는다.")) :
               FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
}

FPinConnectionResponse FOutputPinConnectionPolicy::ValidateConnection(const UEdGraphPin* A,
                                                                      const UEdGraphPin* B)
{
    // 꼭 출력->입력으로 연결이 이루어지라는 법은 없기 때문
    const UEdGraphPin* OutputPin = nullptr;
    if (A->Direction == EGPD_Output)
    {
        OutputPin = A;
    }

    if (B->Direction == EGPD_Output)
    {
        OutputPin = B;
    }

    if (OutputPin)
    {
        return OutputPin->HasAnyConnections() ?
                   FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("출력핀은 동시에 하나의 연결만 허용합니다.")) :
                   FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
    }

    return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
}
#pragma endregion

#pragma region PinConnection
const FPinConnectionResponse UDialogueGraphSchema::CanCreateConnection(const UEdGraphPin* A,
                                                                       const UEdGraphPin* B) const
{
    // 정책 목록을 순회 -> 모든 정책 검사를 통과해야 연결이 허용됨
    // 정책은 .h파일의 ConnectionResponse의 InitializerList에 추가하면 됨
    // TArray<TSharedPtr<FPinConnectionPolicy>> UDialogueGraphSchema::ConnectionPolicies

    FPinConnectionResponse ConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
    for (TSharedPtr PinConnectionPolicy : ConnectionPolicies)
    {
        if (PinConnectionPolicy.IsValid())
        {
            if (ConnectionResponse = PinConnectionPolicy->ValidateConnection(A, B);
                ConnectionResponse.Response == CONNECT_RESPONSE_DISALLOW)
            {
                return ConnectionResponse;
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
