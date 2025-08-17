#include "DialogueEdGraphNodeCreator.h"

#include "DialogueSystemEditorModule.h"
#include "EditorCustomNodeManager.h"
#include "Graph/DialogueGraph.h"
#include "Graph/Node/DialogueCustomNode.h"
#include "Graph/Node/DialogueEdGraphCustomNode.h"
#include "Graph/Node/DialogueEdGraphDialogueLineNode.h"
#include "Graph/Node/DialogueEdGraphEndNode.h"
#include "Graph/Node/DialogueEdGraphSelectNode.h"
#include "Graph/Node/DialogueEdGraphStartNode.h"
#include "Graph/Node/DialogueEditorCustomNodeDefinition.h"
#include "Graph/Node/DialogueEndNode.h"
#include "Graph/Node/DialogueLineNode.h"
#include "Graph/Node/DialogueSelectionNode.h"
#include "Graph/Node/DialogueStartNode.h"

class FDialogueSystemEditorModule;

FDialogueEdGraphNodeCreator::FDialogueEdGraphNodeCreator(UDialogueGraph* InGraph, TMap<UDialogueEdGraphNode*, UDialogueGraphNode*>* EditorToRuntime): Graph(InGraph), EditorToRuntime(EditorToRuntime)
{
}

void FDialogueEdGraphNodeCreator::VisitStartNode(UDialogueEdGraphStartNode* StartNode)
{
    auto RuntimeNode = NewObject<UDialogueStartNode>(Graph);
    EditorToRuntime->Emplace(StartNode, RuntimeNode);

    // 에디터용 GUID를 런타임에서도 사용함 -> 수정 필요
    RuntimeNode->SetNodeID(StartNode->NodeGuid);

    StartNode->CopyTo(RuntimeNode);

    //그래프에 추가
    Graph->AddNode(RuntimeNode);
}

void FDialogueEdGraphNodeCreator::VisitEndNode(UDialogueEdGraphEndNode* EndNode)
{
    auto RuntimeNode = NewObject<UDialogueEndNode>(Graph);
    EditorToRuntime->Emplace(EndNode, RuntimeNode);

    // 에디터용 GUID를 런타임에서도 사용함 -> 수정 필요
    RuntimeNode->SetNodeID(EndNode->NodeGuid);

    EndNode->CopyTo(RuntimeNode);

    //그래프에 추가
    Graph->AddNode(RuntimeNode);
}

void FDialogueEdGraphNodeCreator::VisitSelectNode(UDialogueEdGraphSelectNode* SelectNode)
{
    auto RuntimeNode = NewObject<UDialogueSelectionNode>(Graph);
    EditorToRuntime->Emplace(SelectNode, RuntimeNode);

    // 에디터용 GUID를 런타임에서도 사용함 -> 수정 필요
    RuntimeNode->SetNodeID(SelectNode->NodeGuid);

    SelectNode->CopyTo(RuntimeNode);

    //그래프에 추가
    Graph->AddNode(RuntimeNode);
}

void FDialogueEdGraphNodeCreator::VisitLineNode(UDialogueEdGraphDialogueLineNode* LineNode)
{
    auto RuntimeNode = NewObject<UDialogueLineNode>(Graph);
    EditorToRuntime->Emplace(LineNode, RuntimeNode);

    // 에디터용 GUID를 런타임에서도 사용함 -> 수정 필요
    RuntimeNode->SetNodeID(LineNode->NodeGuid);

    LineNode->CopyTo(RuntimeNode);

    // 그래프에 추가
    Graph->AddNode(RuntimeNode);
}

void FDialogueEdGraphNodeCreator::VisitSceneNode(UDialogueEdGraphSceneNode* SceneNode)
{
    auto RuntimeNode = NewObject<UDialogueLineNode>(Graph);
    EditorToRuntime->Emplace(SceneNode, RuntimeNode);

    // 에디터용 GUID를 런타임에서도 사용함 -> 수정 필요
    RuntimeNode->SetNodeID(SceneNode->NodeGuid);

    // 설정을 복사
    RuntimeNode->SetDialogueSetting(SceneNode->GetDialogueSetting());

    //그래프에 추가
    Graph->AddNode(RuntimeNode);
}

void FDialogueEdGraphNodeCreator::VisitCustomNode(UDialogueEdGraphCustomNode* EventNode)
{
    const auto& Module    = FModuleManager::Get().GetModuleChecked<FDialogueSystemEditorModule>("DialogueSystemEditor");
    const auto Class      = EventNode->GetClass();
    const auto Definition = Module.GetCustomNodeManager()->GetCustomNodeDefinition(Class);

    checkf(Definition, TEXT("현재 모듈에 등록되어 있지 않은 커스텀 노드(%s)를 저장하려 하고 있습니다."), *Class->GetName())

    auto RuntimeNode = NewObject<UDialogueCustomNode>(Graph, Definition->GetRuntimeNodeType());
    EditorToRuntime->Emplace(EventNode, RuntimeNode);

    // 에디터용 GUID를 런타임에서도 사용함 -> 수정 필요
    RuntimeNode->SetNodeID(EventNode->NodeGuid);

    EventNode->CopyTo(RuntimeNode);

    // 사용자가 정의한 방법으로 데이터를 복사
    Definition->InitializeRuntimeNodeWithEditorNode(RuntimeNode, EventNode);

    // 그래프에 추가
    Graph->AddNode(RuntimeNode);
}
