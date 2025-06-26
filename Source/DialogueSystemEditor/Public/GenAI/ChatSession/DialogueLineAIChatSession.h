#pragma once
#include "OpenAI/ChatSessionBase_OpenAI.h"

struct FStructuredOutputDeclaration;
class FOpenAIChatSession_Stream;
class FDialogueLineChatHistory;

/// <summary>
/// 대화 라인 생성을 위한 AI 매니저 클래스
/// FOpenAIChatSession_Stream을 컴포지션으로 관리하며 대화 생성 기능을 제공합니다.
/// DialogueLineCreationPromptDetail안에서만 쓰입니다.
/// </summary>
class FDialogueLineAIChatSession
{
public:
    /// <summary>
    /// AI 매니저 인스턴스를 생성합니다.
    /// </summary>
    /// <returns>생성된 AI 매니저의 공유 포인터</returns>
    static TSharedRef<FDialogueLineAIChatSession> CreateInstance();

    /// <summary>
    /// 소멸자
    /// </summary>
    ~FDialogueLineAIChatSession() = default;

    /// <summary>
    /// 채팅 세션에서 메시지를 전송합니다.
    /// </summary>
    /// <param name="Message">전송할 메시지</param>
    void SendMessage(const FString& Message) const;

    /// <summary>
    /// 텍스트 스트리밍 델리게이트를 반환합니다.
    /// </summary>
    /// <returns>텍스트 스트리밍 델리게이트</returns>
    FOnTextStreamed& GetOnTextStreamedDelegate() const;

    /// <summary>
    /// 요청 실패 델리게이트를 반환합니다.
    /// </summary>
    /// <returns>요청 실패 델리게이트</returns>
    FOnRequestFailedDelegate& GetOnRequestFailedDelegate() const;

    /// <summary>
    /// 요청 완료 델리게이트를 반환합니다.
    /// </summary>
    /// <returns>요청 완료 델리게이트</returns>
    FOnRequestItemReceived& GetOnRequestCompletedDelegate() const;

    void SetBackgroundItems(const TArray<FString>& Backgrounds) const;

    /// <summary>
    /// 시스템 프롬프트를 설정합니다.
    /// </summary>
    /// <param name="InSystemPrompt">설정할 시스템 프롬프트</param>
    void SetSystemPrompt(const FString& InSystemPrompt) const;

    /// <summary>
    /// 감정 프롬프트를 설정합니다.
    /// </summary>
    /// <param name="InEmotionPrompt">설정할 감정 프롬프트</param>
    void SetEmotionPrompt(const FString& InEmotionPrompt) const;

    /// <summary>
    /// 설명 프롬프트를 설정합니다.
    /// </summary>
    /// <param name="InDialogueDescriptionPrompt">설정할 설명 프롬프트</param>
    void SetDialogueDescriptionPrompt(const FString& InDialogueDescriptionPrompt) const;

    /// <summary>
    /// 대화라인 설명 프롬프트를 설정합니다.
    /// </summary>
    /// <param name="InLineDescriptionPrompt">설정할 대화라인 설명 프롬프트</param>
    void SetLineDescriptionPrompt(const FString& InLineDescriptionPrompt) const;

    /// <summary>
    /// 채팅 세션이 유효한지 확인합니다.
    /// </summary>
    /// <returns>채팅 세션이 유효하면 true, 그렇지 않으면 false</returns>
    bool IsValid() const;

    /// <summary>
    /// 생성자 (private - CreateInstance를 통해서만 생성 가능)
    /// </summary>
    FDialogueLineAIChatSession();

private:
    /// <summary>
    /// AI 매니저를 초기화합니다.
    /// </summary>
    /// <returns>초기화 성공 여부</returns>
    bool Initialize();

    /// <summary>
    /// 구조화된 출력 선언을 생성합니다.
    /// </summary>
    /// <returns>구조화된 출력 선언의 공유 포인터</returns>
    static TSharedPtr<FStructuredOutputDeclaration> CreateStructuredOutputDeclaration();

private:
    /// <summary>
    /// OpenAI 채팅 세션 (컴포지션)
    /// </summary>
    TSharedPtr<FOpenAIChatSession_Stream> ChatSession;

    /// <summary>
    /// 채팅 히스토리
    /// </summary>
    TSharedPtr<FDialogueLineChatHistory> ChatHistory;
    FString SystemPrompt;
};
