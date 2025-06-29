// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"
#include "Graph/Node/DialogueEdGraphNode.h"

class UDialogueEdGraphDialogueLineNode;
class SCanvas;
class SMultiLineEditableText;
/**
 * 
 */
class DIALOGUESYSTEMEDITOR_API SDialogueEdGraphLineNode final : public SGraphNode
{
public:
    SLATE_BEGIN_ARGS(SDialogueEdGraphLineNode)
        {
        }

    SLATE_END_ARGS()

    /// <summary>
    /// 시작 노드 위젯을 구성합니다.
    /// </summary>
    /// <param name="InArgs">Slate 인수</param>
    /// <param name="LineNode">시작 노드</param>
    void Construct(const FArguments& InArgs,
                   UEdGraphNode* LineNode);

    /// <summary>
    /// 노드 본문 브러시를 반환합니다.
    /// </summary>
    /// <returns>노드 본문 브러시</returns>
    virtual const FSlateBrush* GetNodeBodyBrush() const override;

    /// <summary>
    /// 핀 위젯들을 생성합니다.
    /// </summary>
    virtual void CreatePinWidgets() override;

    virtual void UpdateGraphNode() override;

    void MakeImageBrush();

    /// <summary>
    /// 노드 제목을 커스터마이징할 때 구현합니다.
    /// 기본 노드 제목 대신 사용자 정의 위젯을 표시하고 싶을 때 사용합니다.
    /// </summary>
    /// <returns>사용자 정의 노드 제목 위젯</returns>
    virtual TSharedRef<SWidget> CreateTitleWidget(TSharedPtr<SNodeTitle> NodeTitle) override;

#pragma region ImplementationIsOptional
    /// <summary>
    /// 노드의 배경(본문) 색상을 커스터마이징해야 할 때 구현합니다.
    /// 특정 상태나 조건에 따라 노드 색상을 변경하고 싶을 때 사용합니다.
    /// </summary>
    // virtual FSlateColor GetNodeBodyColor() const override;

    /// <summary>
    /// 노드 제목 영역의 배경색을 반환합니다.
    /// 노드 상태나 유형에 따라 다른 색상을 표시할 때 재정의합니다.
    /// </summary>
    // virtual FSlateColor GetNodeTitleColor() const override;

    /// <summary>
    /// 노드 제목 아이콘 및 강조 영역의 색상을 반환합니다.
    /// 아이콘 색상 커스터마이징이 필요한 경우 재정의합니다.
    /// </summary>
    // virtual FSlateColor GetNodeTitleIconColor() const override;

    /// <summary>
    /// 노드에 마우스를 올렸을 때 표시되는 툴팁 텍스트를 반환합니다.
    /// 상세한 정보나 사용 방법 안내를 추가할 때 재정의합니다.
    /// </summary>
    // virtual FText GetNodeTooltip() const override;

    /// <summary>
    /// 노드의 본체 영역 아래에 추가 위젯을 생성합니다.
    /// 노드 하단에 버튼이나 상태 표시 등의 요소를 추가하고 싶을 때 재정의합니다.
    /// </summary>
    // virtual void CreateBelowWidgetControls(TSharedPtr<SVerticalBox> MainBox) override;

    /// <summary>
    /// 입력 핀 영역에 '핀 추가' 버튼을 생성합니다.
    /// 동적으로 입력 핀을 추가할 수 있는 노드에서 재정의합니다.
    /// </summary>
    // virtual void CreateInputSideAddButton(TSharedPtr<SVerticalBox> InputBox) override;

    /// <summary>
    /// 출력 핀 영역에 '핀 추가' 버튼을 생성합니다.
    /// 동적으로 출력 핀을 추가할 수 있는 노드에서 재정의합니다.
    /// </summary>
    // virtual void CreateOutputSideAddButton(TSharedPtr<SVerticalBox> OutputBox) override;

    /// <summary>
    /// 고급 핀 표시/숨김을 위한 화살표 버튼을 생성합니다.
    /// 고급 핀 UI를 커스터마이징하고 싶을 때 재정의합니다.
    /// </summary>
    // virtual void CreateAdvancedViewArrow(TSharedPtr<SVerticalBox> MainBox) override;

    /// <summary>
    /// 노드 타이틀 영역의 텍스트 색상을 반환합니다.
    /// 상태나 중요도에 따라 다른 색상을 표시하고 싶을 때 재정의합니다.
    /// </summary>
    // virtual FLinearColor GetNodeTitleTextColor() const override;

    /// <summary>
    /// 핀 아래쪽에 추가 위젯을 생성합니다.
    /// 상태 표시기나 부가 정보를 표시하는 UI 요소를 추가할 때 사용합니다.
    /// </summary>
    // virtual void CreateBelowPinControls(TSharedPtr<SVerticalBox> MainBox) override;

    /// <summary>
    /// 노드 타이틀의 상세 수준을 결정합니다.
    /// 그래프가 축소되었을 때 간소화된 타이틀을 표시하려면 true를 반환하도록 재정의합니다.
    /// </summary>
    // virtual bool UseLowDetailNodeTitles() const override;

    /// <summary>
    /// 핀 이름의 상세 수준을 결정합니다.
    /// 그래프가 축소되었을 때 간소화된 핀 이름을 표시하려면 true를 반환하도록 재정의합니다.
    /// </summary>
    // virtual bool UseLowDetailPinNames() const override;

    /// <summary>
    /// 특정 핀을 UI에서 숨길지 여부를 결정합니다.
    /// 조건부로 핀을 표시하거나 숨기려면 이 메서드를 재정의합니다.
    /// </summary>
    // virtual bool ShouldPinBeHidden(const UEdGraphPin* InPin) const override;

    /// <summary>
    /// 노드의 오류 정보를 갱신합니다.
    /// 노드 유효성 검사 로직이 필요한 경우 재정의하여 구현합니다.
    /// </summary>
    // virtual void UpdateErrorInfo() override;

    /// <summary>
    /// 노드의 코멘트 색상을 결정합니다.
    /// 노드 타입이나 중요도에 따라 다른 코멘트 색상을 지정할 때 재정의합니다.
    /// </summary>
    // virtual FSlateColor GetCommentColor() const override;
#pragma endregion

protected:
    /// <summary>
    /// 노드 콘텐츠 영역을 생성합니다.
    /// </summary>
    /// <returns>노드 콘텐츠 위젯</returns>
    virtual TSharedRef<SWidget> CreateNodeContentArea() override;

    TSharedPtr<SMultiLineEditableText> DialogueLineTextBox;
    TObjectPtr<UDialogueEdGraphDialogueLineNode> DialogueLineNode;
    TSharedPtr<FSlateBrush> SlateBrush;
    TSharedPtr<SCanvas> Canvas;
};
