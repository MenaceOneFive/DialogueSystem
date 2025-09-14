#include "Graph/Node/DialogueEdGraphDialogueLineNode.h"

#include "LevelSequence.h"
#include "Character/DialogueCharacterAsset.h"
#include "EdGraph/EdGraph.h"
#include "Graph/Node/DialogueLineNode.h"
#include "Visitor/AbstractDialogueEdGraphVisitor.h"

UDialogueEdGraphDialogueLineNode::UDialogueEdGraphDialogueLineNode(): SequencePlaySetting()
{
    SequencePlaySetting.bCanSkipThisNode       = true;
    SequencePlaySetting.StateAfterSequencePlay = EStateAfterSequencePlay::Keep;

    DialogueSetting.bShouldShowCursor = false;
    DialogueSetting.EFocusMode        = EFocusMode::UI;
}

void UDialogueEdGraphDialogueLineNode::CopyTo(const TObjectPtr<UDialogueLineNode>& LineNode) const
{
    LineNode->SetWhenSelectThisNodeFunctionName(GetWhenSelectThisNodeFunctionName());
    LineNode->SetCanSelectThisNodeFunctionName(GetCanSelectThisNodeFunctionName());
    LineNode->SetLevelSequenceToPlay(LevelSequenceToPlay);

    FText Name = GetSelectionName();
    LineNode->SetSelectionName(Name);
    LineNode->SetSequencePlaySetting(SequencePlaySetting);
    LineNode->SetDialogueSetting(GetDialogueSetting());
}

FText UDialogueEdGraphDialogueLineNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    if (DialogueCharacterAsset)
    {
        return FText::FromString(DialogueCharacterAsset->Name);
    }
    return FText::FromString(TEXT("캐릭터 선택 필요!"));
}

FLinearColor UDialogueEdGraphDialogueLineNode::GetNodeTitleColor() const
{
    if (!DialogueCharacterAsset)
    {
        return FLinearColor::Red;
    }
    return Super::GetNodeTitleColor();
}

FText UDialogueEdGraphDialogueLineNode::GetTooltipText() const
{
    if (DialogueCharacterAsset)
    {
        // 캐릭터가 설정되어 있는 경우
        return FText::FromString(FString::Format(TEXT("{0}의 대사"), {DialogueCharacterAsset->Name}));
    }
    return FText::FromString(TEXT("캐릭터 에셋을 설정하세요"));
}

FText UDialogueEdGraphDialogueLineNode::GetDefaultSelectionName() const
{
    return NSLOCTEXT("DialoguePlayer", "LineNode", "대사 노드");
}

void UDialogueEdGraphDialogueLineNode::Accept(FAbstractDialogueEdGraphVisitor* Visitor)
{
    Visitor->VisitLineNode(this);
}


void UDialogueEdGraphDialogueLineNode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent); // 부모 클래스 호출

    // 노드가 수정되었음을 알림 (Undo/Redo 및 SGraphNode 업데이트 트리거)
    Modify();

    if (UEdGraph* MyGraph = GetGraph())
    {
        MyGraph->NotifyNodeChanged(this);
    }
}

TSoftObjectPtr<ULevelSequence> UDialogueEdGraphDialogueLineNode::GetLevelSequenceToPlay() const
{
    return LevelSequenceToPlay;
}

void UDialogueEdGraphDialogueLineNode::SetLevelSequenceToPlay(const TSoftObjectPtr<ULevelSequence>& LevelSequence)
{
    LevelSequenceToPlay = LevelSequence;
}

FSequencePlaySetting UDialogueEdGraphDialogueLineNode::GetSequencePlaySetting() const
{
    return SequencePlaySetting;
}

void UDialogueEdGraphDialogueLineNode::SetSequencePlaySetting(const FSequencePlaySetting& InSequencePlaySetting)
{
    this->SequencePlaySetting = InSequencePlaySetting;
}
