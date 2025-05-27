// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"

class UDialogueEdGraphStartNode;

/// <summary>
/// 대화 에디터 그래프의 시작 노드 Slate 위젯
/// </summary>
class SDialogueEdGraphStartNode final : public SGraphNode
{
public:
    SLATE_BEGIN_ARGS(SDialogueEdGraphStartNode)
        {
        }

    SLATE_END_ARGS()

    /// <summary>
    /// 시작 노드 위젯을 구성합니다.
    /// </summary>
    /// <param name="InArgs">Slate 인수</param>
    /// <param name="StartNode">시작 노드</param>
    void Construct(const FArguments& InArgs, UEdGraphNode* StartNode);

    /// <summary>
    /// 노드 본문 브러시를 반환합니다.
    /// </summary>
    /// <returns>노드 본문 브러시</returns>
    virtual const FSlateBrush* GetNodeBodyBrush() const override;

    /// <summary>
    /// 그래프 노드를 업데이트합니다.
    /// </summary>
    virtual void UpdateGraphNode() override;

    /// <summary>
    /// 핀 위젯들을 생성합니다.
    /// </summary>
    virtual void CreatePinWidgets() override;

protected:
    /// <summary>
    /// 노드 콘텐츠 영역을 생성합니다.
    /// </summary>
    /// <returns>노드 콘텐츠 위젯</returns>
    virtual TSharedRef<SWidget> CreateNodeContentArea() override;
};
