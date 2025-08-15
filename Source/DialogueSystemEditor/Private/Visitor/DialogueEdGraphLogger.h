#pragma once
#include "AbstractDialogueEdGraphVisitor.h"

class UDialogueGraphNode;
class UDialogueEdGraphNode;

/// <summary>
/// 각 노드에 대한 로그를 이용한 출력
/// </summary>
class FDialogueEdGraphLogger final : public FAbstractDialogueEdGraphVisitor
{
public:
    explicit FDialogueEdGraphLogger(const TMap<UDialogueEdGraphNode*, UDialogueGraphNode*>* EditorToRuntime);

    virtual void VisitStartNode(UDialogueEdGraphStartNode* StartNode) override;
    virtual void VisitEndNode(UDialogueEdGraphEndNode* EndNode) override;
    virtual void VisitSelectNode(UDialogueEdGraphSelectNode* SelectNode) override;
    virtual void VisitLineNode(UDialogueEdGraphDialogueLineNode* LineNode) override;
    virtual void VisitSceneNode(UDialogueEdGraphSceneNode* SceneNode) override;

private:
    const TMap<UDialogueEdGraphNode*, UDialogueGraphNode*>* EditorToRuntime;
};
