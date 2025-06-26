#include "DialogueEdGraphVisitor.h"

#include "Graph/Node/DialogueEdGraphEndNode.h"
#include "Graph/Node/DialogueEdGraphNode.h"
#include "Graph/Node/DialogueEndNode.h"
#include "Graph/Node/DialogueGraphNode.h"
#include "Graph/Node/DialogueSceneNode.h"
#include "Graph/Node/DialogueSelectionItem.h"
#include "Graph/Node/DialogueSelectionNode.h"
#include "Graph/Node/DialogueStartNode.h"

void FDialogueEdGraphConnector::VisitStartNode(UDialogueEdGraphStartNode* StartNode)
{
    UDialogueStartNode* RuntimeStartNode      = Cast<UDialogueStartNode>(EditorToRuntime[StartNode]);
    const UDialogueGraphNode* NextRuntimeNode = EditorToRuntime[StartNode->GetNextNode()];

    RuntimeStartNode->SetWhenSelectThisNodeFunctionName(StartNode->GetWhenSelectThisNodeFunctionName());
    // 다음 노드 설정 (일대일)
    RuntimeStartNode->SetNextNodeID(NextRuntimeNode->GetNodeID());
}

void FDialogueEdGraphConnector::VisitEndNode(UDialogueEdGraphEndNode* EndNode)
{
    UDialogueEndNode* RuntimeEndNode = Cast<UDialogueEndNode>(EditorToRuntime[EndNode]);
    RuntimeEndNode->SetWhenSelectThisNodeFunctionName(EndNode->GetWhenSelectThisNodeFunctionName());

    // 이전 노드 설정 (다대일)
    TArray<FGuid> PrevNodeGuids;
    Algo::Transform(EndNode->GetPrevNodes(), PrevNodeGuids, [this](const UDialogueEdGraphNode* EditorNode)
    {
        return this->EditorToRuntime[EditorNode]->GetNodeID();
    });
    RuntimeEndNode->SetPrevNodeIDs(PrevNodeGuids);
}

void FDialogueEdGraphConnector::VisitSelectNode(UDialogueEdGraphSelectNode* SelectNode)
{
    UDialogueSelectionNode* RuntimeSelectionNode = Cast<UDialogueSelectionNode>(EditorToRuntime[SelectNode]);
    RuntimeSelectionNode->SetCanSelectThisNodeFunctionName(SelectNode->GetCanSelectThisNodeFunctionName());
    RuntimeSelectionNode->SetWhenSelectThisNodeFunctionName(SelectNode->GetWhenSelectThisNodeFunctionName());

    // 이전 노드 설정 (다대일)
    TArray<FGuid> PrevNodeGuids;
    Algo::Transform(SelectNode->GetPrevNodes(), PrevNodeGuids, [this](const UDialogueEdGraphNode* EditorNode)
    {
        return this->EditorToRuntime[EditorNode]->GetNodeID();
    });
    RuntimeSelectionNode->SetPrevNodeIDs(PrevNodeGuids);

    // 다음 노드에 대한 처리(일대다)
    TArray<FGuid> NextNodeGuids;
    Algo::Transform(SelectNode->GetNextNodes(), NextNodeGuids, [this](const UDialogueEdGraphNode* EditorNode)
    {
        return this->EditorToRuntime[EditorNode]->GetNodeID();
    });
    RuntimeSelectionNode->SetNextNodeIDs(NextNodeGuids);


    TArray<TObjectPtr<UDialogueSelectionItem>> SelectionItems;
    for ( auto NextNodes = SelectNode->GetNextNodes();
          const UDialogueEdGraphNode* NextNode : NextNodes )
    {
        auto SelectionItem = NewObject<UDialogueSelectionItem>(RuntimeSelectionNode);

        // 핀에 해당하는 다음 연결노드를 설정한다.
        SelectionItem->SetNextNodeToVisit(EditorToRuntime[NextNode]);

        // TODO: 연결에 대한 정보를 토대로 선택지 Text초기화하는 로직 작성

        if ( NextNode->IsA<UDialogueEdGraphDialogueLineNode>() )
        {
            const auto SceneNode         = Cast<UDialogueEdGraphDialogueLineNode>(NextNode);
            SelectionItem->SelectionName = SceneNode->DialogueLine;
        }

        if ( NextNode->IsA<UDialogueEdGraphSelectNode>() )
        {
            const auto SelectionNode     = Cast<UDialogueEdGraphSelectNode>(NextNode);
            SelectionItem->SelectionName = FString::Format(TEXT("다른 선택 {0}"), {RuntimeSelectionNode->GetNodeID().ToString()});
        }

        if ( NextNode->IsA<UDialogueEdGraphEndNode>() )
        {
            const auto EndNode           = Cast<UDialogueEdGraphEndNode>(NextNode);
            SelectionItem->SelectionName = TEXT("나가기");
        }

        SelectionItems.Add(SelectionItem);
    }

    RuntimeSelectionNode->SetSelectionItems(SelectionItems);
}

void FDialogueEdGraphConnector::VisitLineNode(UDialogueEdGraphDialogueLineNode* LineNode)
{
    UDialogueSceneNode* RuntimeLineNode = Cast<UDialogueSceneNode>(EditorToRuntime[LineNode]);
    RuntimeLineNode->SetCanSelectThisNodeFunctionName(LineNode->GetCanSelectThisNodeFunctionName());
    RuntimeLineNode->SetWhenSelectThisNodeFunctionName(LineNode->GetWhenSelectThisNodeFunctionName());

    // MovieScene 참조 설정
    if ( LineNode->LevelSequenceToPlay )
    {
        RuntimeLineNode->SetLevelSequenceToPlay(LineNode->LevelSequenceToPlay);
    }

    // 이전 노드 설정 (다대일)
    TArray<FGuid> PrevNodeGuids;
    Algo::Transform(LineNode->GetPrevNodes(), PrevNodeGuids, [this](const UDialogueEdGraphNode* EditorNode)
    {
        return this->EditorToRuntime[EditorNode]->GetNodeID();
    });
    RuntimeLineNode->SetPrevNodeIDs(PrevNodeGuids);

    // 다음 노드에 대한 처리 (일대일)
    const auto RuntimeNextNode = EditorToRuntime[LineNode->GetNextNode()];
    RuntimeLineNode->SetNextNodeID(RuntimeNextNode->GetNodeID());
}

void FDialogueEdGraphConnector::VisitSceneNode(UDialogueEdGraphSceneNode* SceneNode)
{
}


//FDialogueEdGraphLogger

void FDialogueEdGraphLogger::VisitStartNode(UDialogueEdGraphStartNode* StartNode)
{
    const UDialogueStartNode* RuntimeStartNode = Cast<UDialogueStartNode>(EditorToRuntime[StartNode]);
    UE_LOG(LogTemp, Log, TEXT("StartNode %s Next: %s"), *RuntimeStartNode->GetNodeID().ToString(), *RuntimeStartNode->GetNextNodeID().ToString())
}

void FDialogueEdGraphLogger::VisitEndNode(UDialogueEdGraphEndNode* EndNode)
{
    const UDialogueEndNode* RuntimeEndNode = Cast<UDialogueEndNode>(EditorToRuntime[EndNode]);
    FString PrevIds                        = "prevIds:";
    for ( FGuid PrevNodeID : RuntimeEndNode->GetPrevNodeIDs() )
    {
        PrevIds.Append(PrevNodeID.ToString());
        PrevIds.Append("\n");
    }
    UE_LOG(LogTemp, Log, TEXT("EndNode %s Prev:%s"), *RuntimeEndNode->GetNodeID().ToString(), *PrevIds);
}

void FDialogueEdGraphLogger::VisitSelectNode(UDialogueEdGraphSelectNode* SelectNode)
{
    const UDialogueSelectionNode* RuntimeSelectionNode = Cast<UDialogueSelectionNode>(EditorToRuntime[SelectNode]);
    FString PrevIds                                    = "";
    for ( FGuid PrevNodeID : RuntimeSelectionNode->GetPrevNodeIDs() )
    {
        PrevIds.Append(PrevNodeID.ToString());
        PrevIds.Append("\n");
    }
    FString NextIds = "";
    for ( FGuid NextId : RuntimeSelectionNode->GetPrevNodeIDs() )
    {
        NextIds.Append(NextId.ToString());
        NextIds.Append("\n");
    }
    UE_LOG(LogTemp, Log, TEXT("LineNode %s Prev:%s Next:%s"), *RuntimeSelectionNode->GetNodeID().ToString(), *PrevIds, *NextIds);
}

void FDialogueEdGraphLogger::VisitLineNode(UDialogueEdGraphDialogueLineNode* LineNode)
{
    const UDialogueSceneNode* RuntimeLineNode = Cast<UDialogueSceneNode>(EditorToRuntime[LineNode]);
    FString PrevIds                           = "";
    for ( FGuid PrevNodeID : RuntimeLineNode->GetPrevNodeIDs() )
    {
        PrevIds.Append(PrevNodeID.ToString());
        PrevIds.Append("\n");
    }
    UE_LOG(LogTemp, Log, TEXT("LineNode %s Prev:%s Next:%s"), *RuntimeLineNode->GetNodeID().ToString(), *PrevIds, *RuntimeLineNode->GetNextNodeID().ToString());
}

void FDialogueEdGraphLogger::VisitSceneNode(UDialogueEdGraphSceneNode* SceneNode)
{
}
