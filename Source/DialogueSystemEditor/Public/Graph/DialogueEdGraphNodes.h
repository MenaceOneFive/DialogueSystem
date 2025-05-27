// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "DialogueEdGraphNodes.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTextPropertyChanged, const FString&);

/**
 * 
 */
UCLASS(Abstract)
class DIALOGUESYSTEMEDITOR_API UDialogueEdGraphNode : public UEdGraphNode
{
    GENERATED_BODY()

#pragma region PinCategory

public:
    static const FName PC_Flow;
    static const FName PC_Exec;

#pragma endregion

public:
    UDialogueEdGraphNode();
    bool IsAllowRecursiveConnection() const;
    virtual void PostInitProperties() override;
    virtual void AutowireNewNode(UEdGraphPin* FromPin) override;

    void SetNodePosition(int32 InPosX,
                         int32 InPosY);;

protected:
    bool bAllowRecursiveConnection = false;
};

UCLASS()
class DIALOGUESYSTEMEDITOR_API UDialogueEdGraphStartNode : public UDialogueEdGraphNode
{
    GENERATED_BODY()

public:
    virtual void AllocateDefaultPins() override;
};

UCLASS()
class DIALOGUESYSTEMEDITOR_API UDialogueEdGraphEndNode : public UDialogueEdGraphNode
{
    GENERATED_BODY()

public:
    virtual void AllocateDefaultPins() override;
};

UCLASS()
class DIALOGUESYSTEMEDITOR_API UDialogueEdGraphSceneNode : public UDialogueEdGraphNode
{
    GENERATED_BODY()

public:
    virtual void AllocateDefaultPins() override;
};

USTRUCT(BlueprintType)
struct FDialogueLineCreationPrompt
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    FString Description; // (프롬프트 생성용) 대사에 대한 지시사항

    UPROPERTY(EditAnywhere)
    FString Emotion; // 감정에 대한 지시문 생성하거나 직접 작성할 수 있다.
};


UCLASS()
class DIALOGUESYSTEMEDITOR_API UDialogueEdGraphDialogueLineNode : public UDialogueEdGraphSceneNode
{
    GENERATED_BODY()

public:
    // 실제 사용할 대사 (생성하거나 직접 작성할 수 있다.)
    UPROPERTY(EditAnywhere)
    FString DialogueLine;

    // 마지막으로 사용된 프롬프트
    UPROPERTY(EditAnywhere)
    FDialogueLineCreationPrompt CurrentPrompt;

    // 이전에 적용된 프롬프트들
    UPROPERTY(VisibleAnywhere)
    TArray<FDialogueLineCreationPrompt> PromptHistory;
};

UCLASS()
class DIALOGUESYSTEMEDITOR_API UDialogueEdGraphSelectNode : public UDialogueEdGraphNode
{
    GENERATED_BODY()

public:
    virtual void AllocateDefaultPins() override;
    void AddOutputPin();

private:
    int32 PinCounter = 0;
};
