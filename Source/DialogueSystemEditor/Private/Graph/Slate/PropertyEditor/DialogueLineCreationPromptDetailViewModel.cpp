#include "Graph/Slate/PropertyEditor/DialogueLineCreationPromptDetailViewModel.h"

#include "GenAI/ChatSession/DialogueLineAIChatSession.h"
#include "Graph/DialogueGraph.h"
#include "Graph/DialogueGraphAssetFactory.h"
#include "Graph/Node/DialogueEdGraphDialogueLineNode.h"
#include "Graph/Node/DialogueEdGraphNode.h"
#include "Json/JsonUtil.h"

#define LOCTEXT_NAMESPACE "DialogueLineDetails"

auto ExampleSystemPrompt = TEXT(R"(
캐릭터의 말투를 유지하고 게임 세계와 일치하도록 대화를 작성하십시오.

# 배경

게임 시나리오 작가로서 다음 정보를 제공합니다:
- 캐릭터의 생애, 성장 배경, 성격을 포함한 게임 캐릭터의 배경.
- 캐릭터의 이전 대화 및 말투.
- 게임 세계에 대한 정보.
- 생성해야 할 대화의 설명.
- 캐릭터가 말할 때 사용하는 비언어적 및 언어적 표현.

# 작업

- 제공된 정보를 반영하여 대화를 작성하십시오.
- 캐릭터의 말투가 프롬프트에 제공된 것과 일관성이 있도록 하십시오.
- 게임 세계에 대한 설명을 분석하여 대화를 세계와의 일관성을 유지하면서 게임 세계에 맞추십시오.

# 출력 형식

- 응답은 캐릭터의 대사를 담고 제공된 게임 세계 컨텍스트에 매끄럽게 맞춰지는 문단 또는 여러 문단으로 작성되어야 합니다.)");

FDialogueLineCreationPromptDetailViewModel::FDialogueLineCreationPromptDetailViewModel(TSharedRef<IPropertyHandle> CreationPropertyHandle)
    : ChatSession(FDialogueLineAIChatSession::CreateInstance())
{
    if ( !CreationPropertyHandle->IsValidHandle() )
    {
        checkf(false, TEXT("CreationPropertyHandle is invalid in %s"), TEXT(__FUNCTION__));
        return;
    }

    LineDescriptionHandle = CreationPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FDialogueLineCreationPrompt, Description));
    if ( !LineDescriptionHandle.IsValid() || !LineDescriptionHandle->IsValidHandle() )
    {
        checkf(false, TEXT("LineDescriptionHandle is invalid in %s"), TEXT(__FUNCTION__));
        return;
    }

    EmotionHandle = CreationPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FDialogueLineCreationPrompt, Emotion));
    if ( !EmotionHandle.IsValid() || !EmotionHandle->IsValidHandle() )
    {
        checkf(false, TEXT("EmotionHandle is invalid in %s"), TEXT(__FUNCTION__));
        return;
    }

    const TSharedPtr<IPropertyHandle> DialogueLineNodeHandle = CreationPropertyHandle->GetParentHandle();
    if ( !DialogueLineNodeHandle.IsValid() || !DialogueLineNodeHandle->IsValidHandle() )
    {
        checkf(false, TEXT("DialogueLineNodeHandle is invalid in %s"), TEXT(__FUNCTION__));
        return;
    }

    DialogueLineHandle = DialogueLineNodeHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(UDialogueEdGraphDialogueLineNode, DialogueLine));
    if ( !DialogueLineHandle.IsValid() || !DialogueLineHandle->IsValidHandle() )
    {
        checkf(false, TEXT("DialogueLineHandle is invalid in %s"), TEXT(__FUNCTION__));
        return;
    }

    TArray<UObject*> OuterObjects;
    CreationPropertyHandle->GetOuterObjects(OuterObjects);
    if ( OuterObjects.Num() > 0 )
    {
        const UObject* OuterObject = OuterObjects[0];
        if ( const UEdGraph* DialogueGraph = Cast<UEdGraph>(OuterObject->GetOuter()) )
        {
            if ( const UDialogueGraph* DialogueGraphAsset = Cast<UDialogueGraph>(DialogueGraph->GetOuter()) )
            {
                SetDialogueDescription(DialogueGraphAsset->GetDescription());
            }
        }
    }

    if ( !ChatSession.IsValid() )
    {
        checkf(false, TEXT("ChatSession is invalid in %s"), TEXT(__FUNCTION__));
        return;
    }
    ChatSession->SetSystemPrompt(ExampleSystemPrompt);
}

void FDialogueLineCreationPromptDetailViewModel::BindCallbacks()
{
    if ( !ChatSession.IsValid() )
    {
        checkf(false, TEXT("ChatSession is invalid in %s"), TEXT(__FUNCTION__));
        return;
    }

    ChatSession->GetOnTextStreamedDelegate().BindSP(this, &FDialogueLineCreationPromptDetailViewModel::HandleTextStreamingResponse);
    ChatSession->GetOnRequestFailedDelegate().AddSP(this, &FDialogueLineCreationPromptDetailViewModel::HandleRequestFailure);
    ChatSession->GetOnRequestCompletedDelegate().AddSPLambda(this, [this](FOutputItem_OpenAI OutputItem)
    {
        HandleRequestCompletion(OutputItem, DialogueLineHandle);
    });
}

FDialogueLineCreationPromptDetailViewModel::~FDialogueLineCreationPromptDetailViewModel()
{
}

TSharedPtr<FDialogueLineAIChatSession> FDialogueLineCreationPromptDetailViewModel::GetChatSession() const
{
    if ( !ChatSession.IsValid() )
    {
        checkf(false, TEXT("ChatSession is invalid in %s"), TEXT(__FUNCTION__));
        return nullptr;
    }
    return ChatSession;
}

void FDialogueLineCreationPromptDetailViewModel::SetChatSession(const TSharedPtr<FDialogueLineAIChatSession>& InChatSession)
{
    if ( !InChatSession.IsValid() )
    {
        checkf(false, TEXT("InChatSession is invalid in %s"), TEXT(__FUNCTION__));
        return;
    }
    this->ChatSession = InChatSession;
}

FString FDialogueLineCreationPromptDetailViewModel::GetDialogueDescription() const
{
    return DialogueDescription;
}

FString FDialogueLineCreationPromptDetailViewModel::GetLineDescription() const
{
    if ( !LineDescriptionHandle.IsValid() || !LineDescriptionHandle->IsValidHandle() )
    {
        checkf(false, TEXT("LineDescriptionHandle is invalid in %s"), TEXT(__FUNCTION__));
        return "";
    }
    FString Result;
    LineDescriptionHandle->GetValueAsFormattedString(Result);
    return Result;
}

FString FDialogueLineCreationPromptDetailViewModel::GetEmotion() const
{
    if ( !EmotionHandle.IsValid() || !EmotionHandle->IsValidHandle() )
    {
        checkf(false, TEXT("EmotionHandle is invalid in %s"), TEXT(__FUNCTION__));
        return "";
    }
    FString Result;
    EmotionHandle->GetValueAsFormattedString(Result);
    return Result;
}

FString FDialogueLineCreationPromptDetailViewModel::GetUserInstruction() const
{
    return UserInstruction;
}

FString FDialogueLineCreationPromptDetailViewModel::GetDialogueLine() const
{
    if ( !DialogueLineHandle.IsValid() || !DialogueLineHandle->IsValidHandle() )
    {
        checkf(false, TEXT("DialogueLineHandle is invalid in %s"), TEXT(__FUNCTION__));
        return "";
    }
    FString Result;
    DialogueLineHandle->GetValueAsFormattedString(Result);
    return Result;
}

FString FDialogueLineCreationPromptDetailViewModel::GetSystemMessage() const
{
    return SystemMessage;
}

void FDialogueLineCreationPromptDetailViewModel::SetDialogueDescription(const FString& Description)
{
    DialogueDescription = Description;
}

void FDialogueLineCreationPromptDetailViewModel::SetLineDescription(const FString& Description) const
{
    if ( !LineDescriptionHandle.IsValid() || !LineDescriptionHandle->IsValidHandle() )
    {
        checkf(false, TEXT("LineDescriptionHandle is invalid in %s"), TEXT(__FUNCTION__));
        return;
    }
    LineDescriptionHandle->SetValue(Description);
}

void FDialogueLineCreationPromptDetailViewModel::SetEmotion(const FString& Emotion) const
{
    if ( !EmotionHandle.IsValid() || !EmotionHandle->IsValidHandle() )
    {
        checkf(false, TEXT("EmotionHandle is invalid in %s"), TEXT(__FUNCTION__));
        return;
    }
    EmotionHandle->SetValue(Emotion);
}

void FDialogueLineCreationPromptDetailViewModel::SetUserInstruction(const FString& InUserInstruction)
{
    UserInstruction = InUserInstruction;
}

void FDialogueLineCreationPromptDetailViewModel::SetDialogueLine(const FString& DialogueLine) const
{
    if ( !DialogueLineHandle.IsValid() || !DialogueLineHandle->IsValidHandle() )
    {
        checkf(false, TEXT("DialogueLineHandle is invalid in %s"), TEXT(__FUNCTION__));
        return;
    }
    DialogueLineHandle->SetValue(DialogueLine);
}

void FDialogueLineCreationPromptDetailViewModel::SetSystemMessage(const FString& InSystemMessage)
{
    SystemMessage = InSystemMessage;
}

void FDialogueLineCreationPromptDetailViewModel::SetAIOutput(const FString& String)
{
    AIOutput = String;
}

FString FDialogueLineCreationPromptDetailViewModel::GetAIOutput()
{
    return AIOutput;
}

void FDialogueLineCreationPromptDetailViewModel::AppendAIOutput(const FString& String)
{
    AIOutput += String;
}

/// <summary>
/// 사용자가 Enter 키를 눌렀을 때 처리하는 콜백
/// </summary>
/// <param name="_"></param>
/// <param name="InKeyEvent">키 이벤트</param>
/// <returns>이벤트 처리 결과</returns>
FReply FDialogueLineCreationPromptDetailViewModel::HandleEnterKeyPress(const FGeometry& _,
                                                                       const FKeyEvent& InKeyEvent)
{
    if ( InKeyEvent.GetKey() == EKeys::Enter && !InKeyEvent.IsControlDown() )
    {
        if ( ChatSession.IsValid() )
        {
            SendMessage();
        }

        // 기존 프롬프트 입력창을 비운다.
        SetUserInstruction("");

        // 기존 출력창을 비운다.
        AIOutput.Empty();

        return FReply::Handled();
    }
    return FReply::Unhandled();
}

/// <summary>
/// 사용자 지시사항 텍스트가 변경되었을 때 처리하는 콜백
/// </summary>
/// <param name="InText">변경된 텍스트</param>
void FDialogueLineCreationPromptDetailViewModel::OnUserInstructionTextChanged(const FText& InText)
{
    SetUserInstruction(InText.ToString());
}

/// <summary>
/// AI에서 텍스트가 스트리밍되었을 때 처리하는 콜백
/// </summary>
/// <param name="ItemId">아이템 ID</param>
/// <param name="Role">역할</param>
/// <param name="StreamedText">스트리밍된 텍스트</param>
void FDialogueLineCreationPromptDetailViewModel::HandleTextStreamingResponse(const FString ItemId,
                                                                             const FString Role,
                                                                             const FString StreamedText)
{
    AppendAIOutput(StreamedText);
}

/// <summary>
/// AI 요청이 실패했을 때 처리하는 콜백
/// </summary>
/// <param name="Reason">실패 이유</param>
/// <param name="DetailMessage">상세 메시지</param>
void FDialogueLineCreationPromptDetailViewModel::HandleRequestFailure(ERequestFailedReason Reason,
                                                                      const FString DetailMessage)
{
    AppendAIOutput("Error: " + DetailMessage);
}

/// <summary>
/// AI 요청이 완료되었을 때 처리하는 콜백
/// </summary>
/// <param name="OutputItem">출력 아이템</param>
/// <param name="PropertyHandle">대화라인 프로퍼티 핸들</param>
void FDialogueLineCreationPromptDetailViewModel::HandleRequestCompletion(FOutputItem_OpenAI OutputItem,
                                                                         TSharedPtr<IPropertyHandle> PropertyHandle) const
{
    AsyncTask(ENamedThreads::GameThread, [this, PropertyHandle, OutputItem]
    {
        if ( OutputItem.Message.IsSet() &&
             OutputItem.Message.GetValue().Content.Num() > 0 &&
             OutputItem.Message.GetValue().Content[0].TextContent.IsSet()
        )
        {
            const FOutputTextContent_OpenAI Text = OutputItem.Message.GetValue().Content[0].TextContent.GetValue();
            if ( const auto Json = ToJson(&Text.Text);
                Json->HasTypedField(TEXT("DialogueLine"), EJson::String) )
            {
                const FScopedTransaction Transaction(LOCTEXT("ApplyChatResult", "Apply ChatResult"));
                const FString DialogueLine = Json->GetStringField(TEXT("DialogueLine"));
                if ( PropertyHandle.IsValid() && PropertyHandle->IsValidHandle() )
                {
                    PropertyHandle->SetValueFromFormattedString(DialogueLine);
                }
            }
        }
    });
}

void FDialogueLineCreationPromptDetailViewModel::SendMessage() const
{
    if ( const TSharedPtr<FDialogueLineAIChatSession> DialogueLineAIChatSession = ChatSession )
    {
        const FString SystemPrompt           = FString(ExampleSystemPrompt).TrimStartAndEnd();
        const FString TmpDialogueDescription = GetDialogueDescription().TrimStartAndEnd();
        const FString Emotion                = GetEmotion().TrimStartAndEnd();
        const FString TmpDialogueLine        = GetDialogueLine().TrimStartAndEnd();
        const FString TmpUserInstruction     = GetUserInstruction();

        if ( !SystemPrompt.IsEmpty() )
        {
            DialogueLineAIChatSession->SetSystemPrompt(SystemPrompt);
        }

        if ( !TmpDialogueDescription.IsEmpty() )
        {
            DialogueLineAIChatSession->SetDialogueDescriptionPrompt(TmpDialogueDescription);
        }

        if ( !Emotion.IsEmpty() )
        {
            DialogueLineAIChatSession->SetEmotionPrompt(Emotion);
        }

        if ( !TmpDialogueLine.IsEmpty() )
        {
            DialogueLineAIChatSession->SetLineDescriptionPrompt(TmpDialogueLine);
        }

        DialogueLineAIChatSession->SendMessage(TmpUserInstruction);
    }
}

#undef LOCTEXT_NAMESPACE
