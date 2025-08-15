#include "DialogueEdGraphNodeCreator.h"

#include "Graph/DialogueGraph.h"
#include "Graph/Node/DialogueEdGraphDialogueLineNode.h"
#include "Graph/Node/DialogueEdGraphEndNode.h"
#include "Graph/Node/DialogueEdGraphSelectNode.h"
#include "Graph/Node/DialogueEdGraphStartNode.h"
#include "Graph/Node/DialogueEndNode.h"
#include "Graph/Node/DialogueSceneNode.h"
#include "Graph/Node/DialogueSelectionNode.h"
#include "Graph/Node/DialogueStartNode.h"

FDialogueEdGraphNodeCreator::FDialogueEdGraphNodeCreator(UDialogueGraph* InGraph, TMap<UDialogueEdGraphNode*, UDialogueGraphNode*>* EditorToRuntime): Graph(InGraph), EditorToRuntime(EditorToRuntime)
{
}

void FDialogueEdGraphNodeCreator::VisitStartNode(UDialogueEdGraphStartNode* StartNode)
{
    auto RuntimeNode = NewObject<UDialogueStartNode>(Graph);
    EditorToRuntime->Emplace(StartNode, RuntimeNode);
    // 에디터용 GUID를 런타임에서도 사용함 -> 수정 필요
    RuntimeNode->SetNodeID(StartNode->NodeGuid);
    Graph->AddNode(RuntimeNode);
}

void FDialogueEdGraphNodeCreator::VisitEndNode(UDialogueEdGraphEndNode* EndNode)
{
    auto RuntimeNode = NewObject<UDialogueEndNode>(Graph);
    EditorToRuntime->Emplace(EndNode, RuntimeNode);
    // 에디터용 GUID를 런타임에서도 사용함 -> 수정 필요
    RuntimeNode->SetNodeID(EndNode->NodeGuid);
    Graph->AddNode(RuntimeNode);
}

void FDialogueEdGraphNodeCreator::VisitSelectNode(UDialogueEdGraphSelectNode* SelectNode)
{
    auto RuntimeNode = NewObject<UDialogueSelectionNode>(Graph);
    EditorToRuntime->Emplace(SelectNode, RuntimeNode);
    // 에디터용 GUID를 런타임에서도 사용함 -> 수정 필요
    RuntimeNode->SetNodeID(SelectNode->NodeGuid);
    Graph->AddNode(RuntimeNode);
}

void FDialogueEdGraphNodeCreator::VisitLineNode(UDialogueEdGraphDialogueLineNode* LineNode)
{
    auto RuntimeNode = NewObject<UDialogueSceneNode>(Graph);
    EditorToRuntime->Emplace(LineNode, RuntimeNode);
    // 에디터용 GUID를 런타임에서도 사용함 -> 수정 필요
    RuntimeNode->SetNodeID(LineNode->NodeGuid);
    Graph->AddNode(RuntimeNode);
}

void FDialogueEdGraphNodeCreator::VisitSceneNode(UDialogueEdGraphSceneNode* SceneNode)
{
    auto RuntimeNode = NewObject<UDialogueSceneNode>(Graph);
    EditorToRuntime->Emplace(SceneNode, RuntimeNode);
    // 에디터용 GUID를 런타임에서도 사용함 -> 수정 필요
    RuntimeNode->SetNodeID(SceneNode->NodeGuid);
    Graph->AddNode(RuntimeNode);
}
