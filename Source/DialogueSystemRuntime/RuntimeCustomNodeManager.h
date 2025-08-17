// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Graph/Node/DialogueCustomNode.h"

class FDialogueSystemCustomNodeDefinition;
/**
 * 
 */
class DIALOGUESYSTEMRUNTIME_API FRuntimeCustomNodeManager
{
public:
    FRuntimeCustomNodeManager();

    /// <summary>
    /// 커스텀 노드를 런타임에 등록합니다.
    /// </summary>
    /// <param name="Definition">런타임 커스텀 노드에 대한 정의를 담은 구조체</param>
    void RegisterCustomNodeDefinition(const TSharedPtr<FDialogueSystemCustomNodeDefinition>& Definition);

    /// <summary>
    /// 특정 커스텀 노드를 모듈에서 제외합니다.
    /// </summary>
    /// <param name="Class">정의를 제거할 클래스</param>
    void UnregisterCustomNodeDefinition(UClass* Class);

    FDialogueSystemCustomNodeDefinition* GetDefinitionOfCustomNode(UClass* Class) const;

private:
    TMap<TSubclassOf<UDialogueCustomNode>, TSharedPtr<FDialogueSystemCustomNodeDefinition>> CustomNodeDefinitions;
};
