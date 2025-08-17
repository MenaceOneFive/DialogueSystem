#pragma once
#include "DialogueGraphPinConnectionPolicies.h"
#include "DialogueSystemEditorModule.h"
#include "Node/DialogueEdGraphNode.h"
#include "DialogueGraphSchema.generated.h"


class FDialogueGraphSchema_SpawnNode final : public FEdGraphSchemaAction
{
public:
    FDialogueGraphSchema_SpawnNode(const TSubclassOf<UDialogueEdGraphNode>& InSpawnableNodeClass,
                                   const FText& InMenuItemName,
                                   const FText& InMenuDescription,
                                   const FText& InKeywords,
                                   const int32 InGrouping = 0)
        : FEdGraphSchemaAction(FText::FromString("Dialogue"),
                               InMenuItemName,
                               InMenuDescription,
                               InGrouping,
                               InKeywords),
          SpawnableNodeClass(InSpawnableNodeClass)
    {
    }

    virtual UEdGraphNode* PerformAction(UEdGraph* ParentGraph,
                                        UEdGraphPin* FromPin,
                                        const FVector2D Location,
                                        bool bSelectNewNode = true) override;

private:
    TSubclassOf<UDialogueEdGraphNode> SpawnableNodeClass = nullptr;
};


/// <summary>
/// 대화 그래프 스키마
/// </summary>
UCLASS()
class UDialogueGraphSchema : public UEdGraphSchema
{
    GENERATED_BODY()

public: // SchemaContextMenuActions 마우스 우클릭시 호출되는 ContextAction

    // UEdGraphSchema 인터페이스
    /// <summary>
    /// 그래프에서 우클릭하면 열리는 컨텍스트 메뉴 
    /// </summary>
    /// <param name="ContextMenuBuilder"></param>
    virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;

    /// <summary>
    /// 노드를 우클릭하면 열리는 컨텍스트 메뉴 
    /// </summary>
    /// <param name="Menu"></param>
    /// <param name="Context"></param>
    virtual void GetContextMenuActions(UToolMenu* Menu,
                                       UGraphNodeContextMenuContext* Context) const override;
    virtual void OnPinConnectionDoubleCicked(UEdGraphPin* PinA, UEdGraphPin* PinB, const FVector2f& GraphPosition) const override;

    // PinConnection 핀 연결 가능성 확인, 핀 그리기
    /// <summary>
    /// 핀연결이 가능한지 검사<br>
    /// 정책기반으로 연결가능성을 검사합니다.<br>
    /// FPinConnectionPolicy를 확장한 클래스를 정의하고 그 인스턴스를
    /// TArray<TSharedPtr<FPinConnectionPolicy>> UDialogueGraphSchema::ConnectionPolicies에 추가하면 됩니다.
    /// </summary>
    /// <param name="A">핀1</param>
    /// <param name="B">핀2</param>
    /// <returns>핀연결 가능성에 대한 검토 결과</returns>
    virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A,
                                                             const UEdGraphPin* B) const override;

    virtual FConnectionDrawingPolicy* CreateConnectionDrawingPolicy(int32 InBackLayerID,
                                                                    int32 InFrontLayerID,
                                                                    float InZoomFactor,
                                                                    const FSlateRect& InClippingRect,
                                                                    FSlateWindowElementList& InDrawElements,
                                                                    UEdGraph* InGraphObj) const override;

public: // BreakPin : 연결 끊기
    virtual void BreakNodeLinks(UEdGraphNode& TargetNode) const override;

    virtual void BreakPinLinks(UEdGraphPin& TargetPin,
                               bool bSendsNodeNotification) const override;

public: // Getter
    virtual FLinearColor GetPinTypeColor(const FEdGraphPinType& PinType) const override;

private: // Properties
    TArray<TSharedPtr<FPinConnectionPolicy>> ConnectionPolicies
    {
        MakeShared<FRecursivePinConnectionPolicy>(FRecursivePinConnectionPolicy()),
        MakeShared<FDirectionPinConnectionPolicy>(FDirectionPinConnectionPolicy()),
        MakeShared<FOutputPinConnectionPolicy>(FOutputPinConnectionPolicy()),
        MakeShared<FSelectNodeOutputPinDupConnection>()
    };
};
