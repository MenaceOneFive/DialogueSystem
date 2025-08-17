#include "DialogueEdGraphConnector.h"

#include "DialogueSystemEditorModule.h"
#include "DialogueSystemEvent.h"
#include "EditorCustomNodeManager.h"
#include "Graph/Node/DialogueCustomNode.h"
#include "Graph/Node/DialogueEdGraphCustomNode.h"
#include "Graph/Node/DialogueEdGraphDialogueLineNode.h"
#include "Graph/Node/DialogueEdGraphEndNode.h"
#include "Graph/Node/DialogueEdGraphKnotNode.h"
#include "Graph/Node/DialogueEdGraphSelectNode.h"
#include "Graph/Node/DialogueEdGraphStartNode.h"
#include "Graph/Node/DialogueEditorCustomNodeDefinition.h"
#include "Graph/Node/DialogueEndNode.h"
#include "Graph/Node/DialogueLineNode.h"
#include "Graph/Node/DialogueSelectionNode.h"
#include "Graph/Node/DialogueStartNode.h"

///////////////////////////////////////////////////////////////////////////////////////
// FDialogueEdGraphConnector

FDialogueEdGraphConnector::FDialogueEdGraphConnector(TMap<UDialogueEdGraphNode*, UDialogueGraphNode*>* EditorToRuntime): EditorToRuntime(EditorToRuntime)
{
}

void FDialogueEdGraphConnector::VisitStartNode(UDialogueEdGraphStartNode* StartNode)
{
    UDialogueStartNode* RuntimeStartNode = Cast<UDialogueStartNode>(*EditorToRuntime->Find(StartNode));
    RuntimeStartNode->SetWhenSelectThisNodeFunctionName(StartNode->GetWhenSelectThisNodeFunctionName());

    // 다음 노드 설정 (일대일)
    const auto NextNodeGuid = GetNextNodeGuid(StartNode->GetNextNode());
    ensureMsgf(NextNodeGuid!= InvalidGuid, TEXT("시작노드의 다음 노드가 비어 있습니다. "));
    if (NextNodeGuid != InvalidGuid)
    {
        RuntimeStartNode->SetNextNodeID(NextNodeGuid);
    }
}

void FDialogueEdGraphConnector::VisitEndNode(UDialogueEdGraphEndNode* EndNode)
{
    UDialogueEndNode* RuntimeEndNode = Cast<UDialogueEndNode>(*EditorToRuntime->Find(EndNode));

    // 이전 노드 설정 (다대일)
    const TArray<FGuid> PrevNodeGuids = GetPrevNodeGuids(EndNode->GetPrevNodes());
    RuntimeEndNode->SetPrevNodeIDs(PrevNodeGuids);
}

void FDialogueEdGraphConnector::VisitSelectNode(UDialogueEdGraphSelectNode* SelectNode)
{
    UDialogueSelectionNode* RuntimeSelectionNode = Cast<UDialogueSelectionNode>(*EditorToRuntime->Find(SelectNode));

    // 이전 노드 설정 (다대일)
    const TArray<FGuid> PrevNodeGuids = GetPrevNodeGuids(SelectNode->GetPrevNodes());
    RuntimeSelectionNode->SetPrevNodeIDs(PrevNodeGuids);

    // 다음 노드에 대한 처리(일대다)
    const TArray<FGuid> NextNodeGuids = GetNextNodeGuids(SelectNode->GetNextNodes());
    RuntimeSelectionNode->SetNextNodeIDs(NextNodeGuids);
}

void FDialogueEdGraphConnector::VisitLineNode(UDialogueEdGraphDialogueLineNode* LineNode)
{
    UDialogueLineNode* RuntimeLineNode = Cast<UDialogueLineNode>(*EditorToRuntime->Find(LineNode));

    // 이전 노드 설정 (다대일)
    const TArray<FGuid> PrevNodeGuids = GetPrevNodeGuids(LineNode->GetPrevNodes());
    RuntimeLineNode->SetPrevNodeIDs(PrevNodeGuids);

    // 다음 노드에 대한 처리 (일대일)
    RuntimeLineNode->SetNextNodeID(GetNextNodeGuid(LineNode->GetNextNode()));
}

void FDialogueEdGraphConnector::VisitSceneNode(UDialogueEdGraphSceneNode* SceneNode)
{
    checkf(false, TEXT("미구현"))
}

void FDialogueEdGraphConnector::VisitCustomNode(UDialogueEdGraphCustomNode* CustomNode)
{
    // EventNode는 SceneNode의 자손이므로 이전 노드는 다대일 연결, 다음 노드는 일대일 연결이다.
    const auto& Module    = FModuleManager::Get().GetModuleChecked<FDialogueSystemEditorModule>("DialogueSystemEditor");
    const auto Class      = CustomNode->GetClass();
    const auto Definition = Module.GetCustomNodeManager()->GetCustomNodeDefinition(Class);

    checkf(Definition, TEXT("현재 모듈에 등록되어 있지 않은 커스텀 노드(%s)를 저장하려 하고 있습니다."), *Class->GetName())


    if (Definition->GetConnectionType() == EOutgoingConnection::One)
    {
        const auto RuntimeCustomNode = Cast<UDialogueCustomNode>(*EditorToRuntime->Find(CustomNode));

        // 이전 노드 설정 (다대일)
        const TArray<FGuid> PrevNodeGuids = GetPrevNodeGuids(CustomNode->GetPrevNodes());
        RuntimeCustomNode->SetPrevNodeIDs(PrevNodeGuids);

        // 다음 노드에 대한 처리 (일대일)
        RuntimeCustomNode->SetNextNodeID(GetNextNodeGuid(CustomNode->GetNextNode()));
    }
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
        PrevNodeGuids.Add((*EditorToRuntime->Find(PrevEdNode))->GetNodeID());
    }

    TArray<FGuid> Guids;
    Algo::CopyIf(PrevNodeGuids, Guids, [this](const FGuid& Guid)
    {
        return Guid != InvalidGuid;
    });
    return Guids;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
TObjectPtr<const UDialogueEdGraphNode> FDialogueEdGraphConnector::GetNextNode(const TObjectPtr<const UDialogueEdGraphNode>& EdGraphNode) const
{
    if (!EdGraphNode)
    {
        return nullptr;
    }

    auto NextNode = EdGraphNode;
    while (NextNode->IsA<UDialogueEdGraphKnotNode>())
    {
        const auto KnotNode = Cast<UDialogueEdGraphKnotNode>(NextNode);
        NextNode            = KnotNode->GetNextNode();
    }
    return NextNode;
}

FGuid FDialogueEdGraphConnector::GetNextNodeGuid(const TObjectPtr<const UDialogueEdGraphNode>& EdGraphNode) const
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
        return (*EditorToRuntime->Find(NextNode))->GetNodeID();
    }
    return InvalidGuid;
}

TArray<FGuid> FDialogueEdGraphConnector::GetNextNodeGuids(const TArray<TObjectPtr<const UDialogueEdGraphNode>>& EdGraphNodes) const
{
    TArray<FGuid> NextNodeGuids;
    for (const auto& NextNode : EdGraphNodes)
    {
        // 노트 노드는 런타임에 필요 없으므로 정상 노드의 GUID를 찾는다.
        auto CurrentNode = NextNode;
        while (CurrentNode->IsA<UDialogueEdGraphKnotNode>())
        {
            CurrentNode = Cast<UDialogueEdGraphKnotNode>(CurrentNode)->GetNextNode();
        }

        if (CurrentNode)
        {
            NextNodeGuids.Add((*EditorToRuntime->Find(CurrentNode))->GetNodeID());
        }
    }
    return NextNodeGuids;
}
