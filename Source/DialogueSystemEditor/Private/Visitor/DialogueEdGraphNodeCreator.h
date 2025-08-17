#pragma once
#include "AbstractDialogueEdGraphVisitor.h"

class UDialogueGraphNode;
class UDialogueEdGraphNode;
class UDialogueGraph;

/// <summary>
/// 에디터의 노드를 이용해서 런타임 노드를 생성하는 팩토리 클래스
/// </summary>
class FDialogueEdGraphNodeCreator final : public FAbstractDialogueEdGraphVisitor
{
public:
    explicit FDialogueEdGraphNodeCreator(
            UDialogueGraph* InGraph,
            TMap<UDialogueEdGraphNode*, UDialogueGraphNode*>* EditorToRuntime);

    virtual void VisitStartNode(UDialogueEdGraphStartNode* StartNode) override;
    virtual void VisitEndNode(UDialogueEdGraphEndNode* EndNode) override;
    virtual void VisitSelectNode(UDialogueEdGraphSelectNode* SelectNode) override;
    virtual void VisitLineNode(UDialogueEdGraphDialogueLineNode* LineNode) override;
    virtual void VisitSceneNode(UDialogueEdGraphSceneNode* SceneNode) override;
    virtual void VisitCustomNode(UDialogueEdGraphCustomNode* EventNode) override;

private:
    UDialogueGraph* Graph;
    TMap<UDialogueEdGraphNode*, UDialogueGraphNode*>* EditorToRuntime;
};
