// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/DialogueGraphPanelNodeFactory.h"

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


FDialogueGraphPanelNodeFactory::FDialogueGraphPanelNodeFactory()
{
}

FDialogueGraphPanelNodeFactory::~FDialogueGraphPanelNodeFactory()
{
}

TSharedPtr<SGraphNode> FDialogueGraphPanelNodeFactory::CreateNode(UEdGraphNode* Node) const
{
    /// Engine\Source\Editor\GraphEditor\Private\NodeFactory.cpp:113 에 따르면 Chains of responsibility에 따라서
    /// Node의 타입을 체크하여 적절한 노드를 생성해주는 방식이다. 따라서 아래와 같은 Assert를 사용하지 않는 것이 맞다.
    /// check(Node->IsA(UDialogueEdGraphNode::StaticClass()))

    if (Node->IsA<UDialogueEdGraphStartNode>())
    {
        return SNew(SDialogueEdGraphStartNode, Node);
    }
    if (Node->IsA<UDialogueEdGraphEndNode>())
    {
        return SNew(SDialogueEdGraphEndNode, Node);
    }
    if (Node->IsA<UDialogueEdGraphDialogueLineNode>())
    {
        return SNew(SDialogueEdGraphLineNode, Node);
    }
    if (Node->IsA<UDialogueEdGraphSceneNode>())
    {
        return SNew(SDialogueEdGraphSceneNode, Node);
    }
    if (Node->IsA<UDialogueEdGraphSelectNode>())
    {
        return SNew(SDialogueEdGraphSelectNode, Node);
    }
    if (Node->IsA<UDialogueEdGraphKnotNode>())
    {
        return SNew(SGraphNodeKnot, Node);
    }
    if (Node->IsA<UEdGraphNode_Comment>())
    {
        return SNew(SGraphNodeComment, Cast<UEdGraphNode_Comment>(Node));
    }
    return nullptr;
}
