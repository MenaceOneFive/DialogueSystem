// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueSystemRuntimeModule.h"

#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "DialogueSystemRuntime/RuntimeCustomNodeManager.h"
#include "Graph/Node/DialogueCustomNode.h"
#include "Player/DialoguePlayerInstance.h"
#include "Player/LevelSequencePlayerPool.h"
#include "Subsystem/DialoguePlayerSubsystem.h"

UDialogueQTENode::UDialogueQTENode()
{
    DialogueSetting.bShouldShowCursor = false;
    DialogueSetting.EFocusMode        = EFocusMode::UI;
}

FText UDialogueQTENode::GetDefaultSelectionName() const
{
    return NSLOCTEXT("DialoguePlayer", "QTENode", "QTE 노드");
}

TSubclassOf<UDialogueCustomNode> FDialogueQTENodeDefinition::GetRuntimeCustomNodeType() const
{
    return UDialogueQTENode::StaticClass();
}


void FDialogueQTENodeDefinition::EvaluateCustomNode(const UDialogueCustomNode* CustomNode, UObject* Evaluator) const
{
    if (const auto QTENode = Cast<UDialogueQTENode>(CustomNode))
    {
        if (!Evaluator->Implements<UDialogueCustomNodeSequencePlayer>())
        {
            UE_LOG(LogTemp, Error, TEXT("현재 플레이어로는 이 노드를 재생할 수 없습니다."))

            return;
        }
        const TScriptInterface<IDialogueCustomNodeSequencePlayer> SequencePlayerManager(Evaluator);
        const auto                                                Holder = SequencePlayerManager->GetPlayerHolder(CustomNode);

        if (!Holder)
        {
            UE_LOG(LogTemp, Error, TEXT("현재 노드에 대한 시퀀스 플레이 초기화에 실패했습니다."))
        }

        SequencePlayerManager->PlaySequence(CustomNode, Holder);
    }
}

FRuntimeCustomNodeManager* FDialogueSystemRuntimeModule::GetCustomNodeManager() const
{
    return CustomNodeManager.Get();
}

void FDialogueSystemRuntimeModule::StartupModule()
{
    IModuleInterface::StartupModule();

    CustomNodeManager = MakeUnique<FRuntimeCustomNodeManager>();
    CustomNodeManager->RegisterCustomNodeDefinition(MakeShared<FDialogueQTENodeDefinition>());
}

void FDialogueSystemRuntimeModule::ShutdownModule()
{
    IModuleInterface::ShutdownModule();
    CustomNodeManager->UnregisterCustomNodeDefinition(UDialogueQTENode::StaticClass());
}


IMPLEMENT_MODULE(FDialogueSystemRuntimeModule, DialogueSystemRuntime)
