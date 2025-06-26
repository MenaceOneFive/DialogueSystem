#pragma once

class UDialogueGraphNode;
class UDialogueEdGraphNode;
class UDialogueEdGraphSceneNode;
class UDialogueEdGraphDialogueLineNode;
class UDialogueEdGraphSelectNode;
class UDialogueEdGraphEndNode;
class UDialogueEdGraphStartNode;

class FAbstractDialogueEdGraphVisitor
{
public:
    virtual ~FAbstractDialogueEdGraphVisitor() = default;

    virtual void VisitStartNode(UDialogueEdGraphStartNode* StartNode) = 0;
    virtual void VisitEndNode(UDialogueEdGraphEndNode* EndNode) = 0;
    virtual void VisitSelectNode(UDialogueEdGraphSelectNode* SelectNode) = 0;
    virtual void VisitLineNode(UDialogueEdGraphDialogueLineNode* LineNode) = 0;
    virtual void VisitSceneNode(UDialogueEdGraphSceneNode* SceneNode) = 0;
};

/// <summary>
/// 런타임 노드에 연결 관계를 설정하는 Visitor
/// </summary>
class FDialogueEdGraphConnector final : public FAbstractDialogueEdGraphVisitor
{
public:
    explicit FDialogueEdGraphConnector(const TMap<UDialogueEdGraphNode*, UDialogueGraphNode*>& EditorToRuntime)
        : EditorToRuntime(EditorToRuntime)
    {
    }

    virtual void VisitStartNode(UDialogueEdGraphStartNode* StartNode) override;

    virtual void VisitEndNode(UDialogueEdGraphEndNode* EndNode) override;

    virtual void VisitSelectNode(UDialogueEdGraphSelectNode* SelectNode) override;

    virtual void VisitLineNode(UDialogueEdGraphDialogueLineNode* LineNode) override;

    virtual void VisitSceneNode(UDialogueEdGraphSceneNode* SceneNode) override;

private:
    TMap<UDialogueEdGraphNode*, UDialogueGraphNode*> EditorToRuntime;
};

/// <summary>
/// 각 노드에 대한 로그를 이용한 출력
/// </summary>
class FDialogueEdGraphLogger final : public FAbstractDialogueEdGraphVisitor
{
public:
    explicit FDialogueEdGraphLogger(const TMap<UDialogueEdGraphNode*, UDialogueGraphNode*>& EditorToRuntime)
        : EditorToRuntime(EditorToRuntime)
    {
    }

    virtual void VisitStartNode(UDialogueEdGraphStartNode* StartNode) override;
    virtual void VisitEndNode(UDialogueEdGraphEndNode* EndNode) override;
    virtual void VisitSelectNode(UDialogueEdGraphSelectNode* SelectNode) override;
    virtual void VisitLineNode(UDialogueEdGraphDialogueLineNode* LineNode) override;
    virtual void VisitSceneNode(UDialogueEdGraphSceneNode* SceneNode) override;

private:
    TMap<UDialogueEdGraphNode*, UDialogueGraphNode*> EditorToRuntime;
};
