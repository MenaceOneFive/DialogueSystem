// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/DialogueGraphPanelNodeFactory.h"

#include "DialogueSystemEditorModule.h"
#include "EdGraphNode_Comment.h"
#include "SGraphNodeComment.h"
#include "SGraphNodeKnot.h"
#include "Graph/Node/DialogueEdGraphDialogueLineNode.h"
#include "Graph/Node/DialogueEdGraphEndNode.h"
#include "Graph/Node/DialogueEdGraphKnotNode.h"
#include "Graph/Node/DialogueEdGraphSelectNode.h"
#include "Graph/Node/DialogueEdGraphStartNode.h"
#include "Graph/Slate/Node/SDialogueEdGraphEndNode.h"
#include "Graph/Slate/Node/SDialogueEdGraphLineNode.h"
#include "Graph/Slate/Node/SDialogueEdGraphSceneNode.h"
#include "Graph/Slate/Node/SDialogueEdGraphSelectNode.h"
#include "Graph/Slate/Node/SDialogueEdGraphStartNode.h"


FDialogueGraphPanelNodeFactory::FDialogueGraphPanelNodeFactory(): Module(nullptr)
{
    Module = &FModuleManager::GetModuleChecked<FDialogueSystemEditorModule>("DialogueSystemEditor");
}

FDialogueGraphPanelNodeFactory::~FDialogueGraphPanelNodeFactory()
{
}

TSharedPtr<SGraphNode> FDialogueGraphPanelNodeFactory::CreateNode(UEdGraphNode* Node) const
{
    /// Engine\Source\Editor\GraphEditor\Private\NodeFactory.cpp:113 에 따르면 Chains of responsibility에 따라서
    /// Node의 타입을 체크하여 적절한 노드를 생성해주는 방식이다. 따라서 아래와 같은 Assert를 사용하지 않는 것이 맞다.
    /// check(Node->IsA(UDialogueEdGraphNode::StaticClass()))

    // Nullptr를 반환할 수 있습니다.
    TSharedPtr<SGraphNode> GraphNodeWidget = nullptr;

    // 에디터 노드의 클래스 정보를 이용해서 위젯 생성 델리게이트가 있는지 확인합니다.
    const UClass* NodeType = Node->GetClass();
    if (const FOnMakeWidgetForGraphNode Delegate = Module->GetWidgetCreationDelegate(NodeType);
        Delegate.IsBound())
    {
        // 있으면 위젯을 만들어서 반환합니다.
        GraphNodeWidget = Delegate.Execute(Node);
    }

    return GraphNodeWidget;
}
