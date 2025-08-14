// Fill out your copyright notice in the Description page of Project Settings.
#include "Graph/DialogueGraphPinConnectionPolicies.h"

#include "Graph/Node/DialogueEdGraphNode.h"
#include "Graph/Node/DialogueEdGraphSelectNode.h"

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
    if ( A_Node == B_Node )
    {
        if ( A_Node->IsAllowRecursiveConnection() )
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
    if ( A->Direction == EGPD_Output )
    {
        OutputPin = A;
    }

    if ( B->Direction == EGPD_Output )
    {
        OutputPin = B;
    }

    if ( OutputPin )
    {
        if ( OutputPin->HasAnyConnections() )
        {
            if ( OutputPin == A )
            {
                return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_AB, TEXT("출력핀은 동시에 하나의 연결만 허용합니다. 기존 연결은 제거합니다."));
            }
            if ( OutputPin == B )
            {
                return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_AB, TEXT("출력핀은 동시에 하나의 연결만 허용합니다. 기존 연결은 제거합니다."));
            }
        }
        return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
    }

    return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
}

FPinConnectionResponse FSelectNodeOutputPinDupConnection::ValidateConnection(const UEdGraphPin* A,
                                                                             const UEdGraphPin* B)
{
    if ( UDialogueEdGraphSelectNode* SelectNode = Cast<UDialogueEdGraphSelectNode>(A->GetOwningNode()) )
    {
        const auto Result = Algo::FindByPredicate(B->LinkedTo, [SelectNode](UEdGraphPin* Oth)
        {
            if ( !Oth )
            {
                return false;
            }

            if ( const auto ConnectedSelectNode = Cast<UDialogueEdGraphSelectNode>(Oth->GetOwningNode()) )
            {
                if ( SelectNode == ConnectedSelectNode )
                {
                    // 이미 해당 노드와 연결이 성립되어 있다.
                    return true;
                }
            }
            return false;
        });

        // 만약 기존 선택 노드와 연결이 있다면
        if ( Result )
        {
            return FPinConnectionResponse{CONNECT_RESPONSE_DISALLOW, TEXT("이미 선택 노드와 동일한 출력 핀이 있습니다.")};
        }
    }
    // 해당 사항이 없으면 생성가능
    return FPinConnectionResponse{CONNECT_RESPONSE_MAKE, TEXT("")};
}
