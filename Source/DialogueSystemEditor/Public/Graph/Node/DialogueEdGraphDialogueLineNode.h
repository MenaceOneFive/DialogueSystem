#pragma once

#include "CoreMinimal.h"
#include "Graph/Node/DialogueEdGraphSceneNode.h"
#include "DialogueEdGraphDialogueLineNode.generated.h"

class ULevelSequence;
class UDialogueCharacterAsset;
class FAbstractDialogueEdGraphVisitor;

USTRUCT(BlueprintType)
struct FDialogueLineCreationPrompt
{
    GENERATED_BODY()

    // (프롬프트 생성용) 대사에 대한 지시사항
    UPROPERTY(EditAnywhere)
    FString Description;

    // 감정에 대한 지시문 생성하거나 직접 작성할 수 있다.
    UPROPERTY(EditAnywhere)
    FString Emotion;
};

UCLASS()
class DIALOGUESYSTEMEDITOR_API UDialogueEdGraphDialogueLineNode : public UDialogueEdGraphSceneNode
{
    GENERATED_BODY()

public:
    // 대사를 말하는 캐릭터
    UPROPERTY(EditAnywhere)
    UDialogueCharacterAsset* DialogueCharacterAsset;

    // 생성된 MovieScene
    UPROPERTY(EditAnywhere)
    ULevelSequence* LevelSequenceToPlay;

    // 실제 사용할 대사 (생성하거나 직접 작성할 수 있다.)
    UPROPERTY(EditAnywhere)
    FString DialogueLine;

    // 마지막으로 사용된 프롬프트
    UPROPERTY(EditAnywhere)
    FDialogueLineCreationPrompt CurrentPrompt;

    // 이전에 적용된 프롬프트들
    UPROPERTY(VisibleAnywhere)
    TArray<FDialogueLineCreationPrompt> PromptHistory;

public: // UEdGraphNode
    virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    virtual FLinearColor GetNodeTitleColor() const override;
    virtual FText GetTooltipText() const override;

public: // Visitor 패턴
    virtual void Accept(FAbstractDialogueEdGraphVisitor* Visitor) override;

    /// <summary>
    /// 다음 노드를 반환 
    /// </summary>
    /// <returns>다음 노드</returns>
    const UDialogueEdGraphNode* GetNextNode() const;

    /// <summary>
    /// 이전 노드의 목록을 반환 
    /// </summary>
    /// <returns>이전 노드의 목록</returns>
    TArray<const UDialogueEdGraphNode*> GetPrevNodes() const;

public: // UObject 
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
};
