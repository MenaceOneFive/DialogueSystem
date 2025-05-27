// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// 공통 : 현재 창 이름, 이전, 다음(마지막 단계인 경우 완료)
// 1단계 : 대사에 사용할 데이터 체크
//      - 프롬프트
//      - 프롬프트 외에 사용할 데이터
//          - 바로 직전 대사 노드의 대사
//          - Lore
//          - 데이터 저장 상황
// 2단계 : 대사 생성
//      - 생성된 대사
//      - 추가 지시 프롬프트
//          - 프롬프트 텍스트 박스
//          - 전송 버튼
// 3단계 : 음성
// 4단계 : 페이스 모션
// 5단계 : 모션 (Mixamo 같은 페이지에서 바로 Import 지원)
// 6단계 : 나머지 UMovieScene 설정
// 창 내용 설정

using FWizardStepTuple = TTuple<TSharedRef<SWidget>, FText>;

// TODO: Widget 플러그인으로 이동
class DIALOGUESYSTEMEDITOR_API SWizardWidget final : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SWizardWidget)
        {
        }

        SLATE_ARGUMENT(TArray<FWizardStepTuple>, WidgetToAdd)

    SLATE_END_ARGS()

    void Construct(const FArguments& Arguments,
                   TArray<FWizardStepTuple> InWidgetToAdd);

private:
    TSharedPtr<SWidget> Content;

    TSharedPtr<SWidgetSwitcher> WizardStepSwitcher;
    TSharedPtr<STextBlock> StepIndicator;
    
    // 버튼에 대한 참조
    TSharedPtr<SButton> PrevButton;
    TSharedPtr<SButton> NextButton;

    // 버튼 콜백
    FReply OnPrevClicked() const;
    FReply OnNextClicked() const;

    // 마법사의 각 단계
    TArray<FWizardStepTuple> WidgetToAdd;
    
    void UpdateStepIndicatorText() const;
};
