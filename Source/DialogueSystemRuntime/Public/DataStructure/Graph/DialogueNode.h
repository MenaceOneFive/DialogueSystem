#pragma once
#include "DialogueNode.generated.h"

class ULevelSequence;
class IDialogueGraphVisitor;


UCLASS(BlueprintType, Abstract)
class DIALOGUESYSTEMRUNTIME_API UDialogueGraphNode : public UObject
{
    GENERATED_BODY()

public:
    virtual void BeginDestroy() override
    {
        UE_LOG(LogMemory, Display, TEXT("UDialogueGraphNode"))
        Super::BeginDestroy();
    };

public:
    /// 파생 클래스에서 Visitor의 어느 기능을 통해서 처리할지 정함
    /// 만약 이 클래스의 파생 클래스를 작성한다면, 파생 클래스에 특화된 코드를 Visitor에 구현해야 한다.
    /// <param name="Visitor">이 노드의 데이터를 사용할 Visitor구현</param>
    virtual void VisitNode(IDialogueGraphVisitor* Visitor) PURE_VIRTUAL(UDialogueNode::VisitNode,);

    FGuid GetNodeID() const;
    FGuid GetNextNodeID() const;
    FGuid GetPrevNodeID() const;

    /// 에디터에서 컴파일하고 저장한 뒤에 그 어떠한 수정 행위도 허용하지 않음
    /// 런타임에 이 메서드를 호출하지 말 것
#if WITH_EDITORONLY_DATA

public:
    void SetNodeID(const FGuid& InNodeGuid);

    void SetNextNodeID(const FGuid& InNextNodeID);

    void SetPrevNodeID(const FGuid& InPrevNodeID);
#endif

protected:
    FGuid NodeID;     // 이 노드의 고유 ID
    FGuid NextNodeID; // 다음 노드의 ID
    FGuid PrevNodeID; // 이전 노드의 ID
};

using DialogueLineID = uint64; // TODO: 별도의 정의 파일로 이동


USTRUCT(BlueprintType) // TODO: 별도의 에디터 구현
struct FDialogueSelectionItem
{
    GENERATED_BODY()

    DialogueLineID ID; // 대사 테이블 접근용(대사마다 고유함)
    FGuid NodeID;      // 
    // 조건 검사용 콜백
};

// 대화의 시작을 나타내는 노드
UCLASS(BlueprintType)
class DIALOGUESYSTEMRUNTIME_API UDialogueStartNode : public UDialogueGraphNode
{
    GENERATED_BODY()

public:
    virtual void VisitNode(IDialogueGraphVisitor* Visitor) override;
};

// 선택지를 표시하는 노드 
UCLASS(BlueprintType)
class DIALOGUESYSTEMRUNTIME_API UDialogueSelectionNode : public UDialogueGraphNode
{
    GENERATED_BODY()

public:
    virtual void VisitNode(IDialogueGraphVisitor* Visitor) override;
    TArray<FDialogueSelectionItem> GetSelectionItems() const;

#if WITH_EDITORONLY_DATA
    void SetSelectionItems(const TArray<FDialogueSelectionItem>& Items);
#endif

protected:
    UPROPERTY()
    TArray<FDialogueSelectionItem> SelectionItems;
};

// LevelSequence를 재생하는 노드
UCLASS(BlueprintType)
class DIALOGUESYSTEMRUNTIME_API UDialogueSceneNode : public UDialogueGraphNode
{
    GENERATED_BODY()

public:
    virtual void VisitNode(IDialogueGraphVisitor* Visitor) override;
    ULevelSequence* GetLevelSequenceToPlay() const;

#if WITH_EDITORONLY_DATA
    void SetLevelSequenceToPlay(ULevelSequence* const InLevelSequenceToPlay);
#endif

protected:
    UPROPERTY()
    ULevelSequence* LevelSequenceToPlay = nullptr;
};

// 이벤트를 실행하는 노드 
UCLASS(BlueprintType)
class DIALOGUESYSTEMRUNTIME_API UDialogueEventNode : public UDialogueGraphNode
{
    GENERATED_BODY()

public:
    virtual void VisitNode(IDialogueGraphVisitor* Visitor) override;
};


// 종료 노드
UCLASS(BlueprintType)
class DIALOGUESYSTEMRUNTIME_API UDialogueEndNode : public UDialogueGraphNode
{
    GENERATED_BODY()

public:
    virtual void VisitNode(IDialogueGraphVisitor* Visitor) override;
};
