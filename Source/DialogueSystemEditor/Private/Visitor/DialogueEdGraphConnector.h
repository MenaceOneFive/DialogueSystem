#pragma once
#include "AbstractDialogueEdGraphVisitor.h"

class UDialogueGraphNode;
class UDialogueEdGraphNode;

/// <summary>
/// 런타임 노드에 연결 관계를 설정하는 Visitor
/// </summary>
class FDialogueEdGraphConnector final : public FAbstractDialogueEdGraphVisitor
{
public: // FAbstractDialogueEdGraphVisitor
    explicit FDialogueEdGraphConnector(TMap<UDialogueEdGraphNode*, UDialogueGraphNode*>* EditorToRuntime);

    virtual void VisitStartNode(UDialogueEdGraphStartNode* StartNode) override;

    virtual void VisitEndNode(UDialogueEdGraphEndNode* EndNode) override;

    virtual void VisitSelectNode(UDialogueEdGraphSelectNode* SelectNode) override;

    virtual void VisitLineNode(UDialogueEdGraphDialogueLineNode* LineNode) override;

    virtual void VisitSceneNode(UDialogueEdGraphSceneNode* SceneNode) override;

    virtual void VisitCustomNode(UDialogueEdGraphCustomNode* CustomNode) override;

private:
    TArray<FGuid> GetPrevNodeGuids(const TArray<TObjectPtr<const UDialogueEdGraphNode>>& EdGraphNodes);
    TObjectPtr<const UDialogueEdGraphNode> GetNextNode(const TObjectPtr<const UDialogueEdGraphNode>& EdGraphNode) const;
    FGuid GetNextNodeGuid(const TObjectPtr<const UDialogueEdGraphNode>& EdGraphNode) const;
    TArray<FGuid> GetNextNodeGuids(const TArray<TObjectPtr<const UDialogueEdGraphNode>>& EdGraphNodes) const;

private:
    TMap<UDialogueEdGraphNode*, UDialogueGraphNode*>* EditorToRuntime;
    FGuid InvalidGuid = FGuid();
};
