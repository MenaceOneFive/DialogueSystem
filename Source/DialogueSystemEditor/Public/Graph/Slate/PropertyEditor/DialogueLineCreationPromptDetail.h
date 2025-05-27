// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct FOutputItem_OpenAI;
enum class ERequestFailedReason : uint8;
// Forward Declarations
class FDialogueLineAIChatSession;
class SMultiLineEditableTextBox;

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
    // 콜백 함수들
    /// <summary>
    /// 설명이 변경되었을 때 호출되는 콜백
    /// </summary>
    /// <param name="DescriptionHandle">설명 프로퍼티 핸들</param>
    void OnDescriptionChanged(TSharedPtr<IPropertyHandle> DescriptionHandle);

    /// <summary>
    /// 감정이 변경되었을 때 호출되는 콜백
    /// </summary>
    /// <param name="EmotionHandle">감정 프로퍼티 핸들</param>
    void OnEmotionChanged(TSharedPtr<IPropertyHandle> EmotionHandle);

    /// <summary>
    /// 사용자가 Enter 키를 눌렀을 때 처리하는 콜백
    /// </summary>
    /// <param name="InKeyEvent">키 이벤트</param>
    /// <returns>이벤트 처리 결과</returns>
    FReply HandleEnterKeyPress(const FKeyEvent& InKeyEvent);

    /// <summary>
    /// 사용자 지시사항 텍스트가 변경되었을 때 처리하는 콜백
    /// </summary>
    /// <param name="InText">변경된 텍스트</param>
    void OnUserInstructionTextChanged(const FText& InText);

    /// <summary>
    /// AI에서 텍스트가 스트리밍되었을 때 처리하는 콜백
    /// </summary>
    /// <param name="ItemId">아이템 ID</param>
    /// <param name="Role">역할</param>
    /// <param name="StreamedText">스트리밍된 텍스트</param>
    void HandleTextStreamingResponse(FString ItemId,
                                     FString Role,
                                     FString StreamedText) const;

    /// <summary>
    /// AI 요청이 실패했을 때 처리하는 콜백
    /// </summary>
    /// <param name="Reason">실패 이유</param>
    /// <param name="DetailMessage">상세 메시지</param>
    void HandleRequestFailure(ERequestFailedReason Reason,
                              FString DetailMessage) const;

    /// <summary>
    /// AI 요청이 완료되었을 때 처리하는 콜백
    /// </summary>
    /// <param name="OutputItem">출력 아이템</param>
    /// <param name="PropertyHandle">대화라인 프로퍼티 핸들</param>
    void HandleRequestCompletion(FOutputItem_OpenAI OutputItem,
                                 TSharedPtr<IPropertyHandle> PropertyHandle) const;

    // 위젯 생성 함수들
    /// <summary>
    /// AI 출력 텍스트 패널을 생성합니다.
    /// </summary>
    /// <param name="DescriptionVerticalBox">설명 수직 박스</param>
    void CreateOutputTextPanel(const TSharedPtr<SVerticalBox>& DescriptionVerticalBox);

    /// <summary>
    /// 감정 위젯을 생성합니다.
    /// </summary>
    /// <param name="EmotionHandle">감정 프로퍼티 핸들</param>
    /// <param name="DescriptionVerticalBox">설명 수직 박스</param>
    void CreateEmotionWidget(const TSharedPtr<IPropertyHandle>& EmotionHandle,
                             const TSharedPtr<SVerticalBox>& DescriptionVerticalBox);

    /// <summary>
    /// 설명 위젯을 생성합니다.
    /// </summary>
    /// <param name="DescriptionHandle">설명 프로퍼티 핸들</param>
    /// <param name="DescriptionVerticalBox">설명 수직 박스</param>
    void CreateDescriptionWidget(const TSharedPtr<IPropertyHandle>& DescriptionHandle,
                                 const TSharedPtr<SVerticalBox>& DescriptionVerticalBox);

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
    void CreateDialogueDescriptionWidget(TSharedRef<IPropertyHandle> PropertyHandle,
                                         const TSharedPtr<SVerticalBox>& DescriptionVerticalBox);

private:
    void SendMessage() const;

private:
    /// <summary>
    /// AI 매니저 (FOpenAIChatSession_Stream을 컴포지션으로 관리)
    /// </summary>
    TSharedPtr<FDialogueLineAIChatSession> ChatSession;

    /// <summary>
    /// AI의 답변 (대사를 제외한)의 텍스트 박스 위젯
    /// </summary>
    TSharedPtr<SMultiLineEditableTextBox> AITextResponseTextBox;

    /// <summary>
    /// AI의 답변 (대사를 제외한)
    /// </summary>
    FString AIOutput;

    /// <summary>
    /// 대화 전체에 대한 설명
    /// </summary>
    FString DialogueDescription;

    /// <summary>
    /// 상황에 대한 설명 (저장됨)
    /// </summary>
    FString LineDescription;

    /// <summary>
    /// 감정에 대한 설명 (저장됨)
    /// </summary>
    FString Emotion;

    /// <summary>
    /// 사용자가 추가적으로 제공하는 힌트(저장되지 않음)
    /// </summary>
    FString UserInstruction;

    /// <summary>
    /// 사용자가 추가적으로 제공하는 힌트의 텍스트 박스
    /// </summary>
    TSharedPtr<SMultiLineEditableTextBox> UserInstructionTextBox;

    /// <summary>
    /// 텍스트 래핑 너비
    /// </summary>
    float WrapTextAt = 450.f;
};
