// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueLineCreationPromptDetailViewModel.h"

class SMultiLineEditableTextBox;
struct FOutputItem_OpenAI;
enum class ERequestFailedReason : uint8;

/// <summary>
/// FDialogueLineCreationPrompt 타입에 대한 커스텀 프로퍼티 에디터
/// </summary>
class FDialogueLineCreationPromptDetailTypeCustomization final : public IPropertyTypeCustomization
{
public:
    /// <summary>
    /// 커스터마이제이션 인스턴스를 생성합니다.
    /// </summary>
    /// <returns>생성된 인스턴스의 공유 참조</returns>
    static TSharedRef<IPropertyTypeCustomization> MakeInstance();

    // IPropertyTypeCustomization 인터페이스
    /// <summary>
    /// 프로퍼티 헤더를 커스터마이즈합니다.
    /// </summary>
    /// <param name="PropertyHandle">프로퍼티 핸들</param>
    /// <param name="HeaderRow">헤더 행</param>
    /// <param name="CustomizationUtils">커스터마이제이션 유틸리티</param>
    virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle,
                                 FDetailWidgetRow& HeaderRow,
                                 IPropertyTypeCustomizationUtils& CustomizationUtils) override;

    /// <summary>
    /// 프로퍼티 자식 요소들을 커스터마이즈합니다.
    /// </summary>
    /// <param name="PropertyHandle">프로퍼티 핸들</param>
    /// <param name="ChildBuilder">자식 빌더</param>
    /// <param name="CustomizationUtils">커스터마이제이션 유틸리티</param>
    virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
                                   IDetailChildrenBuilder& ChildBuilder,
                                   IPropertyTypeCustomizationUtils& CustomizationUtils) override;

protected:
    // 위젯 생성 함수들
    /// <summary>
    /// AI 출력 텍스트 패널을 생성합니다.
    /// </summary>
    /// <param name="DescriptionVerticalBox">설명 수직 박스</param>
    void CreateOutputTextPanel(const TSharedPtr<SVerticalBox>& DescriptionVerticalBox);

    /// <summary>
    /// 감정 위젯을 생성합니다.
    /// </summary>
    /// <param name="DescriptionVerticalBox">설명 수직 박스</param>
    void CreateEmotionWidget(const TSharedPtr<SVerticalBox>& DescriptionVerticalBox) const;

    /// <summary>
    /// 설명 위젯을 생성합니다.
    /// </summary>
    /// <param name="DescriptionVerticalBox">설명 수직 박스</param>
    void CreateDescriptionWidget(const TSharedPtr<SVerticalBox>& DescriptionVerticalBox) const;

    /// <summary>
    /// 사용자 지시사항 위젯을 생성합니다.
    /// </summary>
    /// <param name="DescriptionVerticalBox">설명 수직 박스</param>
    void CreateUserInstructionWidget(const TSharedPtr<SVerticalBox>& DescriptionVerticalBox);

    /// <summary>
    /// 대화 라인 생성 대화창의 설명을 생성하는 함수
    /// </summary>
    /// <param name="PropertyHandle">대화라인 프로퍼티 핸들</param>
    /// <param name="DescriptionVerticalBox">설명 수직 박스</param>
    void CreateDialogueDescriptionWidget(
        const TSharedPtr<IPropertyHandle>& PropertyHandle,
        const TSharedPtr<SVerticalBox>& DescriptionVerticalBox) const;

private:
    // ViewModel을 통한 상태 관리
    TSharedPtr<FDialogueLineCreationPromptDetailViewModel> ViewModel;
    // UI 위젯들
    TSharedPtr<SMultiLineEditableTextBox> AITextResponseTextBox;
    TSharedPtr<SMultiLineEditableTextBox> UserInstructionTextBox;
    float WrapTextAt = 450.f;
};
