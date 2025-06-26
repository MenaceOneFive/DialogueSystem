#include "Graph/DialogueBlueprintUtils.h"
#include "K2Node_FunctionEntry.h"
#include "K2Node_FunctionResult.h"
#include "Graph/DialogueGraphDirector.h"
#include "Kismet2/BlueprintEditorUtils.h"


EFunctionCreationResult FDialogueBlueprintUtils::CreateWhenSelectedFunction(UBlueprint* Blueprint,
                                                                            const FName FunctionName)
{
    check(Blueprint)
    return CreateFunctionGraph(Blueprint, EFunctionType::WhenSelectThisNode, FunctionName);
}

EFunctionCreationResult FDialogueBlueprintUtils::CreateCanSelectThisNodeFunction(UBlueprint* Blueprint,
                                                                                 const FName FunctionName)
{
    check(Blueprint)
    return CreateFunctionGraph(Blueprint, EFunctionType::CanSelectThisNode, FunctionName);
}

UEdGraph* FDialogueBlueprintUtils::FindFunctionGraph(UBlueprint* Blueprint,
                                                     FName FunctionGraphName)
{
    const auto Graph = Blueprint->FunctionGraphs.FindByPredicate([FunctionGraphName](const TObjectPtr<UEdGraph> Graph)
    {
        UE_LOG(LogTemp, Log, TEXT(""))
        return Graph && Graph->GetName().Equals(FunctionGraphName.ToString());
    });

    return Graph ? *Graph : nullptr;
}

EFunctionCreationResult FDialogueBlueprintUtils::CreateFunctionGraph(UBlueprint* Blueprint,
                                                                     EFunctionType FunctionType,
                                                                     FName FunctionGraphName)
{
    // 이 이름에 해당하는 함수 그래프가 있는지 확인
    const auto Graph = Blueprint->FunctionGraphs.FindByPredicate([FunctionGraphName](const TObjectPtr<UEdGraph> Graph)
    {
        UE_LOG(LogTemp, Log, TEXT(""))
        return Graph && Graph->GetName().Equals(FunctionGraphName.ToString());
    });

    UEdGraph* FunctionGraph = Graph ? Graph->Get() : nullptr;

    if ( FunctionGraph )
    {
        return EFunctionCreationResult::Failed_DuplicatedName;
    }

    // 찾을 수 없는 경우 그래프를 새로 만든다.
    FunctionGraph = FBlueprintEditorUtils::CreateNewGraph(Blueprint, FunctionGraphName, UEdGraph::StaticClass(), UEdGraphSchema_K2::StaticClass());

    UFunction* FunctionSignature = nullptr;

    switch ( FunctionType )
    {
    case EFunctionType::CanSelectThisNode:
        FunctionSignature = Blueprint->GeneratedClass->FindFunctionByName(UDialogueGraphDirector::CanSelectThisNodeSignatureName);
        break;
    case EFunctionType::WhenSelectThisNode:
        FunctionSignature = Blueprint->GeneratedClass->FindFunctionByName(UDialogueGraphDirector::WhenSelectThisNodeSignatureName);
        break;
    default:
        FunctionSignature = nullptr;
    }

    checkf(FunctionSignature, TEXT("Director클래스에서 해당 메서드에 대한 시그니처를 찾을 수 없습니다."))

    FBlueprintEditorUtils::AddFunctionGraph<UFunction>(Blueprint, FunctionGraph, false, FunctionSignature);

    FunctionGraph->Modify();

    // 함수의 시작노드의 위치를 최적의 위치로 이동
    const auto EntryNode = FunctionGraph->Nodes.FindByPredicate([](const TObjectPtr<UEdGraphNode>& Node)
    {
        return Node.Get()->IsA<UK2Node_FunctionEntry>();
    });
    check(EntryNode)
    EntryNode->Get()->SetPosition(FunctionGraph->GetGoodPlaceForNewNode());

    // 함수의 종료노드의 위치를 최적의 위치로 이동
    if ( FunctionType == EFunctionType::CanSelectThisNode )
    {
        auto EntryNodePosition = EntryNode->Get()->GetPosition();

        // EntryNode에서 Size를 얻으려 했는데 0이 반환되므로 하드코딩 처리
        EntryNodePosition.Set(EntryNodePosition.X + 700.f, EntryNodePosition.Y);
        const auto ResultNode = FunctionGraph->Nodes.FindByPredicate([](const TObjectPtr<UEdGraphNode>& Node)
        {
            return Node.Get()->IsA<UK2Node_FunctionResult>();
        });
        check(ResultNode)
        ResultNode->Get()->SetPosition(EntryNodePosition);
    }

    return EFunctionCreationResult::OK;
}
