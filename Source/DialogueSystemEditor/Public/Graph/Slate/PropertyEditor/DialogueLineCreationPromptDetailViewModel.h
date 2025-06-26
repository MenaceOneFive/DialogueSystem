#pragma once

#include "CoreMinimal.h"

struct FOutputItem_OpenAI;
enum class ERequestFailedReason : uint8;
class FDialogueLineAIChatSession;

/**
 * DialogueLineCreationPromptDetail의 뷰모델 (로직 및 상태 관리)
 */
class FDialogueLineCreationPromptDetailViewModel
        : public TSharedFromThis<FDialogueLineCreationPromptDetailViewModel>
{
public:
    explicit FDialogueLineCreationPromptDetailViewModel(TSharedRef<IPropertyHandle> CreationPropertyHandle);
    void BindCallbacks();
    ~FDialogueLineCreationPromptDetailViewModel();

    TSharedPtr<FDialogueLineAIChatSession> GetChatSession() const;
    void SetChatSession(const TSharedPtr<FDialogueLineAIChatSession>& InChatSession);

    /// <summary>
    /// Getters
    /// </summary>
    FString GetDialogueDescription() const;
    FString GetLineDescription() const;
    FString GetEmotion() const;
    FString GetUserInstruction() const;
    FString GetDialogueLine() const;
    FString GetSystemMessage() const;
    FString GetAIOutput();

    /// <summary>
    /// Setters
    /// </summary>
    void SetDialogueDescription(const FString& Description);
    void SetLineDescription(const FString& Description) const;
    void SetEmotion(const FString& Emotion) const;
    void SetUserInstruction(const FString& InUserInstruction);
    void SetDialogueLine(const FString& DialogueLine) const;
    void SetSystemMessage(const FString& InSystemMessage);
    void SetAIOutput(const FString& String);
    void AppendAIOutput(const FString& String);

    /// <summary>
    /// 사용자가 Enter 키를 눌렀을 때 처리하는 콜백
    /// </summary>
    /// <param name="InKeyEvent">키 이벤트</param>
    /// <returns>이벤트 처리 결과</returns>
    FReply HandleEnterKeyPress(const FGeometry& _,
                               const FKeyEvent& InKeyEvent);

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
                                     FString StreamedText);

    /// <summary>
    /// AI 요청이 실패했을 때 처리하는 콜백
    /// </summary>
    /// <param name="Reason">실패 이유</param>
    /// <param name="DetailMessage">상세 메시지</param>
    void HandleRequestFailure(ERequestFailedReason Reason,
                              FString DetailMessage);

    /// <summary>
    /// AI 요청이 완료되었을 때 처리하는 콜백
    /// </summary>
    /// <param name="OutputItem">출력 아이템</param>
    /// <param name="PropertyHandle">대화라인 프로퍼티 핸들</param>
    void HandleRequestCompletion(FOutputItem_OpenAI OutputItem,
                                 TSharedPtr<IPropertyHandle> PropertyHandle) const;
    void SendMessage() const;

private :
    // AI 채팅 세션
    TSharedPtr<FDialogueLineAIChatSession> ChatSession;

    TSharedPtr<IPropertyHandle> LineDescriptionHandle;
    TSharedPtr<IPropertyHandle> EmotionHandle;
    TSharedPtr<IPropertyHandle> DialogueLineHandle;
    FString UserInstruction;
    FString SystemMessage;
    FString DialogueDescription;
    FString AIOutput;
};
