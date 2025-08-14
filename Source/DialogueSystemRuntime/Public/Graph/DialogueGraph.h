// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DialogueGraph.generated.h"

class UDialogueStartNode;
class UDialogueGraphNode;

UCLASS(BlueprintType)
class DIALOGUESYSTEMRUNTIME_API UDialogueGraph : public UObject
{
    GENERATED_BODY()

public:
    virtual void BeginDestroy() override;

public: // Getter
    TObjectPtr<UBlueprint> GetBlueprintInstance() const;
    TObjectPtr<UClass> GetBlueprintClass() const;
    FString GetDirectorBlueprintName() const;

#if WITH_EDITOR
    void SetBlueprintInstance(UBlueprint* InBlueprintInstance);
#endif

#if WITH_EDITOR

public:
    /// <summary>
    /// 그래프에 새 노드를 추가한다.
    /// </summary>
    /// <param name="GraphNode">추가할 그래프 노드</param>
    /// <returns>삽입 성공여부</returns>
    UFUNCTION(BlueprintCallable)
    bool AddNode(UDialogueGraphNode* GraphNode);

    void SetStartNode(TObjectPtr<UDialogueStartNode> InStartNode);

    void Clear();

    /// <summary>
    /// 현재 대화의 설명을 반환합니다.
    /// </summary>
    /// <returns>현재 대화의 설명</returns>
    FString GetDescription() const;

    /// <summary>
    /// 현재 대화에 대한 편집자의 메모를 반환합니다. 
    /// </summary>
    /// <returns>현재 대화에 대한 메모</returns>
    FString GetMemo() const;

#endif

public: // 런타임 그래프 사용
    /// <summary>
    /// 노드를 반환합니다.
    /// </summary>
    /// <param name="NodeID">노드의 GUID</param>
    /// <returns>그래프 노드(nullptr일 수 있음)</returns>
    UFUNCTION(BlueprintCallable)
    UDialogueGraphNode* GetNode(const FGuid NodeID);

    /// <summary>
    /// 노드가 그래프에 포함되어 있는지 확인합니다. 
    /// </summary>
    /// <param name="GraphNode">그래프에 포함되어 있는지 확인할 노드</param>
    /// <returns>포함 여부</returns>
    UFUNCTION(BlueprintCallable)
    bool ContainsNode(const UDialogueGraphNode* GraphNode) const;

    /// <summary>
    /// 노드가 그래프에 포함되어 있는지 확인합니다. 
    /// </summary>
    /// <param name="NodeID">노드의 ID</param>
    /// <returns>포함 여부</returns>
    TObjectPtr<const UDialogueGraphNode> GetNode(const FGuid NodeID) const;

    /// <summary>
    /// 현재 그래프의 시작노드를 반환합니다.
    /// </summary>
    /// <returns>현재 그래프의 시작노드</returns>
    UFUNCTION(BlueprintCallable)
    UDialogueStartNode* GetStartNode() const;

    TArray<TObjectPtr<const UDialogueGraphNode>> GetNodes() const;

private:
    void OnDirectorRecompiled(TObjectPtr<UBlueprint> InBlueprint);


#if WITH_EDITORONLY_DATA

public: /// 에디터타임 그래프
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UAssetUserData> AssetUserData; //DialogueEdGraph;

protected:
    // 이 대화의 상황 ( AI가 참고함 )
    UPROPERTY(EditAnywhere)
    FString Description;

    // 편집자의 코멘터리 ( AI가 참고하지 않음 )
    UPROPERTY(EditAnywhere)
    FString Memo;

#endif

private:
    // DirectorBlueprint의 런타임 인스턴스
    UPROPERTY() 
    TObjectPtr<UBlueprint> BlueprintInstance;

    // DirectorBlueprint의 정의
    UPROPERTY() 
    TObjectPtr<UClass> BlueprintClass;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UDialogueStartNode> StartNode;

    UPROPERTY(VisibleAnywhere)
    TMap<FGuid, UDialogueGraphNode*> Nodes;
};
