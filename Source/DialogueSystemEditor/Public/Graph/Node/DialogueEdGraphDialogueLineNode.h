#pragma once

#include "CoreMinimal.h"
#include "Graph/Node/DialogueEdGraphSceneNode.h"
#include "Graph/Node/SequencePlayCapability.h"
#include "DialogueEdGraphDialogueLineNode.generated.h"

class UDialogueLineNode;
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
                                                                  , public ISequencePlayCapability
{
    GENERATED_BODY()

public:
    UDialogueEdGraphDialogueLineNode();

    void CopyTo(const TObjectPtr<UDialogueLineNode>& LineNode) const;

    // 대사를 말하는 캐릭터
    UPROPERTY(EditAnywhere)
    TObjectPtr<UDialogueCharacterAsset> DialogueCharacterAsset;

    // 생성된 MovieScene
    UPROPERTY(EditAnywhere)
    TSoftObjectPtr<ULevelSequence> LevelSequenceToPlay;

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

    virtual FText GetDefaultSelectionName() const override;;

    // Visitor 패턴
    virtual void Accept(FAbstractDialogueEdGraphVisitor* Visitor) override;

public: // UObject 
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

public: // ISequencePlayCapability
    virtual TSoftObjectPtr<ULevelSequence> GetLevelSequenceToPlay() const override;
    virtual void SetLevelSequenceToPlay(const TSoftObjectPtr<ULevelSequence>& LevelSequence) override;
    virtual FSequencePlaySetting GetSequencePlaySetting() const override;
    virtual void SetSequencePlaySetting(const FSequencePlaySetting& InSequencePlaySetting) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FSequencePlaySetting SequencePlaySetting;
};
