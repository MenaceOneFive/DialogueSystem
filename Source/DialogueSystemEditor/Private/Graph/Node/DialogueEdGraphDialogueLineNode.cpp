#include "Graph/Node/DialogueEdGraphDialogueLineNode.h"
#include "Character/DialogueCharacterAsset.h"
#include "EdGraph/EdGraph.h"
#include "Visitor/AbstractDialogueEdGraphVisitor.h"

FText UDialogueEdGraphDialogueLineNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    if ( DialogueCharacterAsset )
    {
        return FText::FromString(DialogueCharacterAsset->Name);
    }
    return FText::FromString(TEXT("캐릭터 선택 필요!"));
}

FLinearColor UDialogueEdGraphDialogueLineNode::GetNodeTitleColor() const
{
    if ( !DialogueCharacterAsset )
    {
        return FLinearColor::Red;
    }
    return Super::GetNodeTitleColor();
}

FText UDialogueEdGraphDialogueLineNode::GetTooltipText() const
{
    if ( DialogueCharacterAsset )
    {
        // 캐릭터가 설정되어 있는 경우
        return FText::FromString(FString::Format(TEXT("{0}의 대사"), {DialogueCharacterAsset->Name}));
    }
    return FText::FromString(TEXT("캐릭터 에셋을 설정하세요"));
}

void UDialogueEdGraphDialogueLineNode::Accept(FAbstractDialogueEdGraphVisitor* Visitor)
{
    Visitor->VisitLineNode(this);
}

TObjectPtr<const UDialogueEdGraphNode> UDialogueEdGraphDialogueLineNode::GetNextNode() const
{
    // LineNode 출력 1개만 갖고 있다고 가정한다.
    UEdGraphPin* PinToNext = *Algo::FindByPredicate(Pins, [](const UEdGraphPin* Pin)
    {
        return Pin->Direction == EGPD_Output;
    });

    // 연결이 없는 경우 nullptr를 반환
    if ( !PinToNext )
    {
        return nullptr;
    }

    // 출력에 연결되는 입력은 1개만 있다고 가정한다.
    if ( !PinToNext->LinkedTo.IsEmpty() )
    {
        if ( const UEdGraphPin* LinkedTo = *(PinToNext->LinkedTo.begin()) )
        {
            return Cast<UDialogueEdGraphNode>(LinkedTo->GetOwningNode());
        }
    }
    return nullptr;
}


TArray<TObjectPtr<const UDialogueEdGraphNode>> UDialogueEdGraphDialogueLineNode::GetPrevNodes() const
{
    const auto InputPin = Algo::FindByPredicate(Pins, [](const UEdGraphPin* Pin)
    {
        return Pin->Direction == EGPD_Input;
    });

    checkf(InputPin, TEXT("InputPin은 무조건 존재해야 합니다."))

    // 한 핀에 들어 올 수 있는 입력은 여러개일 수 있다.
    TArray<TObjectPtr<const UDialogueEdGraphNode>> Output;

    // 입력핀과 연결되어 있는 핀들의 소유주를 목록화 해서 반환
    Algo::Transform((*InputPin)->LinkedTo, Output, [](const UEdGraphPin* Pin)
    {
        return Cast<UDialogueEdGraphNode>(Pin->GetOwningNode());
    });
    return Output;
}

void UDialogueEdGraphDialogueLineNode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent); // 부모 클래스 호출

    // 노드가 수정되었음을 알림 (Undo/Redo 및 SGraphNode 업데이트 트리거)
    Modify();

    if ( UEdGraph* MyGraph = GetGraph() )
    {
        MyGraph->NotifyNodeChanged(this);
    }
}
