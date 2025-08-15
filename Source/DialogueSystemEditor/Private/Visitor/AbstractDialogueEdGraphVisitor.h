#pragma once

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
