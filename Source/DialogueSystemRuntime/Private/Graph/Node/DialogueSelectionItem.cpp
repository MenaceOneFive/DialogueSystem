#include "Graph/Node/DialogueSelectionItem.h"
#include "Graph/DialogueGraph.h"
#include "Graph/DialogueGraphDirector.h"
#include "Graph/DialogueGraphVisitor.h"
#include "Graph/Node/DialogueConditionalNode.h"
#include "Graph/Node/DialogueGraphNode.h"

bool UDialogueSelectionItem::CanSelectThisItem()
{
    if ( CanSelectThisItemDelegate.IsBound() )
    {
        return CanSelectThisItemDelegate.Execute();
    }
    return true;
}

void UDialogueSelectionItem::SelectThisItem()
{
    check(VisitNextNodeDelegate.IsBound())
    (void)VisitNextNodeDelegate.ExecuteIfBound();
}


void UDialogueSelectionItem::InitializeForExecution(UDialogueGraphDirector* Director,
                                                    TScriptInterface<IRuntimeDialogueGraphVisitor> Visitor)
{
    TObjectPtr<UDialogueGraphNode> NodeToVisit = NextNodeToVisit;

    // 기존에 바인딩된 정보를 제거
    VisitNextNodeDelegate.Unbind();
    CanSelectThisItemDelegate.Unbind();

    // 새로 바인딩
    VisitNextNodeDelegate.BindLambda([NodeToVisit, Visitor]()
    {
        if ( const auto VisitorInterface = Visitor.GetInterface();
            VisitorInterface && NodeToVisit->IsValidLowLevel() )
        {
            check(NodeToVisit->IsValidLowLevel())
            NodeToVisit->Accept(Visitor);
        }
    });

    CanSelectThisItemDelegate.BindLambda([Director, NodeToVisit]()
    {
        if ( const auto ConditionalNode = Cast<UDialogueConditionalNode>(NodeToVisit) )
        {
            if ( const auto FunctionName = ConditionalNode->GetCanSelectThisNodeFunctionName(); !FunctionName.IsEqual(NAME_None) )
            {
                checkf(Director->IsValidLowLevel(), TEXT("Director의 생명주기에 문제가 있습니다."))

                const auto Graph = NodeToVisit->GetTypedOuter<UDialogueGraph>();
                checkf(Graph, TEXT("Node와 에셋 사이의 관계설정이 잘못되어 있습니다. 노드 > 그래프 > 에셋 생성시 Outer를 잘 설정했는지 확인하세요"))

                const auto Function = Graph->GetBlueprintClass()->FindFunctionByName(FunctionName);
                checkf(Function, TEXT("에디터에서는 존재했던 함수가 런타임에서는 확인되지 않는 현상입니다."))

                return Director->CanVisitNode(Function, NodeToVisit);
            }
        }
        return true;
    });
}

#if WITH_EDITOR
void UDialogueSelectionItem::SetNextNodeToVisit(UDialogueGraphNode* NextNode)
{
    NextNodeToVisit = NextNode;
}
#endif
