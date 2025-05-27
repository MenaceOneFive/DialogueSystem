#pragma once
#include "DialogueGraphSchema.generated.h"

class UDialogueEdGraphNode;

class FDialogueGraphSchema_SpawnNode : public FEdGraphSchemaAction
{
public:
#pragma region SchemaContextMenuActions
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

#pragma endregion

private:
    TSubclassOf<UDialogueEdGraphNode> SpawnableNodeClass = nullptr;
};

#pragma region PinConnectionPolicies    // 핀간 연결이 적절한지 검사하는 정책들을 정의
class FPinConnectionPolicy
{
public:
    virtual ~FPinConnectionPolicy() = default;
    virtual FPinConnectionResponse ValidateConnection(const UEdGraphPin* A,
                                                      const UEdGraphPin* B);
};

// 핀의 입력/출력에 상관 없이 양 끝단이 같은 노드, 즉 자기 자신에 대한 연결을 막는 정책
// 만약 재귀적 연결을 허용하고 싶다면 노드의 bAllowRecursiveConnection를 true로 설정해야 한다.
class FRecursivePinConnectionPolicy final : public FPinConnectionPolicy
{
public:
    virtual FPinConnectionResponse ValidateConnection(const UEdGraphPin* A,
                                                      const UEdGraphPin* B) override;
};

// 두 핀의 방향은 같을 수 없다.
class FDirectionPinConnectionPolicy final : public FPinConnectionPolicy
{
public:
    virtual FPinConnectionResponse ValidateConnection(const UEdGraphPin* A,
                                                      const UEdGraphPin* B) override;
};

// 출력핀은 하나의 커넥션만 가질 수 있다.
class FOutputPinConnectionPolicy final : public FPinConnectionPolicy
{
public:
    virtual FPinConnectionResponse ValidateConnection(const UEdGraphPin* A,
                                                      const UEdGraphPin* B) override;
};
#pragma endregion

// 대화 그래프 스키마
UCLASS()
class UDialogueGraphSchema : public UEdGraphSchema
{
    GENERATED_BODY()

#pragma region SchemaContextMenuActions // 마우스 우클릭시 호출되는 ContextAction

public:
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
#pragma endregion

#pragma region PinConnection // 핀 연결 가능성 확인, 핀 그리기

public:
    virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A,
                                                             const UEdGraphPin* B) const override;

    virtual FConnectionDrawingPolicy* CreateConnectionDrawingPolicy(int32 InBackLayerID,
                                                                    int32 InFrontLayerID,
                                                                    float InZoomFactor,
                                                                    const FSlateRect& InClippingRect,
                                                                    class FSlateWindowElementList& InDrawElements,
                                                                    class UEdGraph* InGraphObj) const override;
#pragma endregion

#pragma region BreakPin

public:
    virtual void BreakNodeLinks(UEdGraphNode& TargetNode) const override;

    virtual void BreakPinLinks(UEdGraphPin& TargetPin,
                               bool bSendsNodeNotification) const override;
#pragma endregion

    // Getter
public:
    virtual FLinearColor GetPinTypeColor(const FEdGraphPinType& PinType) const override;

    // Properties
private:
    TArray<TSharedPtr<FPinConnectionPolicy>> ConnectionPolicies{
        MakeShared<FRecursivePinConnectionPolicy>(FRecursivePinConnectionPolicy()),
        MakeShared<FDirectionPinConnectionPolicy>(FDirectionPinConnectionPolicy()),
        MakeShared<FOutputPinConnectionPolicy>(FOutputPinConnectionPolicy())
    };
};
