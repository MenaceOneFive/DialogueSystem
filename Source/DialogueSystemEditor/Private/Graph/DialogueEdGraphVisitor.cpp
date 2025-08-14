#include "DialogueEdGraphVisitor.h"

#include "Graph/Node/DialogueEdGraphDialogueLineNode.h"
#include "Graph/Node/DialogueEdGraphEndNode.h"
#include "Graph/Node/DialogueEdGraphKnotNode.h"
#include "Graph/Node/DialogueEdGraphNode.h"
#include "Graph/Node/DialogueEdGraphSelectNode.h"
#include "Graph/Node/DialogueEdGraphStartNode.h"
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
    const auto NextNodeGuid = GetNextNodeGuid(StartNode->GetNextNode());
    ensureMsgf(NextNodeGuid!= InvalidGuid, TEXT("시작노드의 다음 노드가 비어 있습니다. "));
    if (NextNodeGuid != InvalidGuid)
    {
        RuntimeStartNode->SetNextNodeID(NextRuntimeNode->GetNodeID());
    }
}

void FDialogueEdGraphConnector::VisitEndNode(UDialogueEdGraphEndNode* EndNode)
{
    UDialogueEndNode* RuntimeEndNode = Cast<UDialogueEndNode>(EditorToRuntime[EndNode]);
    RuntimeEndNode->SetWhenSelectThisNodeFunctionName(EndNode->GetWhenSelectThisNodeFunctionName());

    // 이전 노드 설정 (다대일)
    const TArray<FGuid> PrevNodeGuids = GetPrevNodeGuids(EndNode->GetPrevNodes());

    RuntimeEndNode->SetPrevNodeIDs(PrevNodeGuids);
}

void FDialogueEdGraphConnector::VisitSelectNode(UDialogueEdGraphSelectNode* SelectNode)
{
    UDialogueSelectionNode* RuntimeSelectionNode = Cast<UDialogueSelectionNode>(EditorToRuntime[SelectNode]);
    RuntimeSelectionNode->SetCanSelectThisNodeFunctionName(SelectNode->GetCanSelectThisNodeFunctionName());
    RuntimeSelectionNode->SetWhenSelectThisNodeFunctionName(SelectNode->GetWhenSelectThisNodeFunctionName());

    // 이전 노드 설정 (다대일)
    const TArray<FGuid> PrevNodeGuids = GetPrevNodeGuids(SelectNode->GetPrevNodes());
    RuntimeSelectionNode->SetPrevNodeIDs(PrevNodeGuids);

    // 다음 노드에 대한 처리(일대다)
    TArray<FGuid> NextNodeGuids;
    Algo::Transform(SelectNode->GetNextNodes(), NextNodeGuids, [this](const UDialogueEdGraphNode* EditorNode)
    {
        // 노트 노드는 런타임에 필요 없으므로 정상 노드의 GUID를 찾는다.
        auto CurrentNode = EditorNode;
        while (CurrentNode->IsA<UDialogueEdGraphKnotNode>())
        {
            CurrentNode = Cast<UDialogueEdGraphKnotNode>(CurrentNode)->GetNextNode();
        }

        if (CurrentNode)
        {
            return this->EditorToRuntime[CurrentNode]->GetNodeID();
        }
        return InvalidGuid;
    });

    TArray<FGuid> Guids;
    Algo::CopyIf(NextNodeGuids, Guids, [this](const FGuid& Guid)
    {
        // 유효하지 않은 연결은 제거
        return Guid != InvalidGuid;
    });

    RuntimeSelectionNode->SetNextNodeIDs(Guids);


    TArray<TObjectPtr<UDialogueSelectionItem>> SelectionItems;
    for (auto NextNodes = SelectNode->GetNextNodes();
         const UDialogueEdGraphNode* NextNode : NextNodes)
    {
        const UDialogueEdGraphNode* Node = NextNode;
        if (Node->IsA<UDialogueEdGraphKnotNode>())
        {
            while (Node && Node->IsA<UDialogueEdGraphKnotNode>())
            {
                Node = Cast<UDialogueEdGraphKnotNode>(Node)->GetNextNode();
            }
        }

        auto SelectionItem = NewObject<UDialogueSelectionItem>(RuntimeSelectionNode);

        // 핀에 해당하는 다음 연결노드를 설정한다.
        SelectionItem->SetNextNodeToVisit(EditorToRuntime[Node]);

        // TODO: 연결에 대한 정보를 토대로 선택지 Text초기화하는 로직 작성

        if (Node->IsA<UDialogueEdGraphDialogueLineNode>())
        {
            const auto SceneNode         = Cast<UDialogueEdGraphDialogueLineNode>(Node);
            SelectionItem->SelectionName = SceneNode->DialogueLine;
        }

        if (Node->IsA<UDialogueEdGraphSelectNode>())
        {
            const auto SelectionNode     = Cast<UDialogueEdGraphSelectNode>(Node);
            SelectionItem->SelectionName = FString::Format(TEXT("다른 선택 {0}"), {RuntimeSelectionNode->GetNodeID().ToString()});
        }

        if (Node->IsA<UDialogueEdGraphEndNode>())
        {
            const auto EndNode           = Cast<UDialogueEdGraphEndNode>(Node);
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
    if (LineNode->LevelSequenceToPlay)
    {
        RuntimeLineNode->SetLevelSequenceToPlay(LineNode->LevelSequenceToPlay);
    }

    // 이전 노드 설정 (다대일)
    const TArray<FGuid> PrevNodeGuids = GetPrevNodeGuids(LineNode->GetPrevNodes());
    RuntimeLineNode->SetPrevNodeIDs(PrevNodeGuids);

    // 다음 노드에 대한 처리 (일대일)
    RuntimeLineNode->SetNextNodeID(GetNextNodeGuid(LineNode->GetNextNode()));
}

void FDialogueEdGraphConnector::VisitSceneNode(UDialogueEdGraphSceneNode* SceneNode)
{
}

TArray<FGuid> FDialogueEdGraphConnector::GetPrevNodeGuids(const TArray<TObjectPtr<const UDialogueEdGraphNode>>& EdGraphNodes)
{
    TArray<FGuid> PrevNodeGuids;
    for (const auto PrevEdNode : EdGraphNodes)
    {
        if (PrevEdNode->IsA<UDialogueEdGraphKnotNode>())
        {
            const auto KnotNode = Cast<UDialogueEdGraphKnotNode>(PrevEdNode);
            if (const auto PrevNodesOfKnotNode = GetPrevNodeGuids(KnotNode->GetPrevNodes());
                !PrevNodesOfKnotNode.IsEmpty())
            {
                PrevNodeGuids.Append(PrevNodesOfKnotNode);
            }
            continue;
        }
        PrevNodeGuids.Add(EditorToRuntime[PrevEdNode]->GetNodeID());
    }

    TArray<FGuid> Guids;
    Algo::CopyIf(PrevNodeGuids, Guids, [this](const FGuid& Guid)
    {
        return Guid != InvalidGuid;
    });
    return Guids;
}

FGuid FDialogueEdGraphConnector::GetNextNodeGuid(const TObjectPtr<const UDialogueEdGraphNode>& EdGraphNode)
{
    if (!EdGraphNode)
    {
        return InvalidGuid;
    }

    auto NextNode = EdGraphNode;
    while (NextNode->IsA<UDialogueEdGraphKnotNode>())
    {
        const auto KnotNode = Cast<UDialogueEdGraphKnotNode>(NextNode);
        NextNode            = KnotNode->GetNextNode();
    }
    if (NextNode)
    {
        return EditorToRuntime[NextNode]->GetNodeID();
    }
    return InvalidGuid;
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
    for (FGuid PrevNodeID : RuntimeEndNode->GetPrevNodeIDs())
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
    for (FGuid PrevNodeID : RuntimeSelectionNode->GetPrevNodeIDs())
    {
        PrevIds.Append(PrevNodeID.ToString());
        PrevIds.Append("\n");
    }
    FString NextIds = "";
    for (FGuid NextId : RuntimeSelectionNode->GetNextNodeIDs())
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
    for (FGuid PrevNodeID : RuntimeLineNode->GetPrevNodeIDs())
    {
        PrevIds.Append(PrevNodeID.ToString());
        PrevIds.Append("\n");
    }
    UE_LOG(LogTemp, Log, TEXT("LineNode %s Prev:%s Next:%s"), *RuntimeLineNode->GetNodeID().ToString(), *PrevIds, *RuntimeLineNode->GetNextNodeID().ToString());
}

void FDialogueEdGraphLogger::VisitSceneNode(UDialogueEdGraphSceneNode* SceneNode)
{
}
