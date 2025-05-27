// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/DialoguePlayerSubsystem.h"

#include "LevelSequence.h"
#include "DataStructure/Graph/DialogueNode.h"
#include "Player/DialoguePlayerInstance.h"

void UDialoguePlayerSubsystem::StartDialogue(ULevelSequence* MasterLevelSequence,
                                             const UDialogueGraph* InDialogueGraph)
{
    ensureMsgf(!DialogueInstance, TEXT("이미 재생중인 대화가 있음, 뭔가 문제가 있는 것이니 반드시 확인!"));

    if (DialogueInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("이미 재생중인 대화가 있음, 뭔가 문제가 있는 것이니 반드시 확인!"))
        //테스트는 할 수 있게 정리는 해줌 ㅇㅇ
        StopDialogue();
        RemoveDialogue();
    }

    DialogueInstance = NewObject<UDialoguePlayerInstance>(this);
    checkf(MasterLevelSequence, TEXT("MasterLevelSequence는 nullptr일 수 없습니다."));
    DialogueInstance->Initialize(MasterLevelSequence, InDialogueGraph);
    DialogueInstance->Play();
}

void UDialoguePlayerSubsystem::StartDialogue2(ULevelSequence* MasterLevelSequence,
                                              TArray<ULevelSequence*> LevelSequences)
{
#pragma region GraphCreation
    UDialogueGraph* NewDialogueGraph = NewObject<UDialogueGraph>(this);
    NewDialogueGraph->Initialize();
#pragma region CreateNodes
    UDialogueStartNode* StartNode  = NewDialogueGraph->GetStartNode();
    UDialogueSceneNode* SceneNode1 = NewObject<UDialogueSceneNode>(NewDialogueGraph);
    UDialogueSceneNode* SceneNode2 = NewObject<UDialogueSceneNode>(NewDialogueGraph);
    UDialogueSceneNode* SceneNode3 = NewObject<UDialogueSceneNode>(NewDialogueGraph);
    UDialogueSceneNode* SceneNode4 = NewObject<UDialogueSceneNode>(NewDialogueGraph);
    UDialogueEndNode* EndNode      = NewObject<UDialogueEndNode>(NewDialogueGraph);
#pragma endregion

#pragma region SetSequence
    ULevelSequence* InLevelSequenceToPlay = LoadObject<ULevelSequence>(NewDialogueGraph,
                                                                       TEXT("/Game/Cinematics/Sequences/Sequence/shot/shot0010/shot0010_01.shot0010_01"));
    SceneNode1->SetLevelSequenceToPlay(InLevelSequenceToPlay);
    ULevelSequence* LevelSequenceToPlay = LoadObject<ULevelSequence>(NewDialogueGraph,
                                                                     TEXT("/Game/Cinematics/Sequences/Sequence/shot/shot0020/shot0020_01.shot0020_01"));
    SceneNode2->SetLevelSequenceToPlay(LevelSequenceToPlay);
    ULevelSequence* SequenceToPlay = LoadObject<ULevelSequence>(NewDialogueGraph,
                                                                TEXT("/Game/Cinematics/Sequences/Sequence/shot/shot0030/shot0030_01.shot0030_01"));
    SceneNode3->SetLevelSequenceToPlay(SequenceToPlay);
    ULevelSequence* ToPlay = LoadObject<ULevelSequence>(NewDialogueGraph,
                                                        TEXT("/Game/Cinematics/Sequences/Sequence/shot/shot0040/shot0040_01.shot0040_01"));
    SceneNode4->SetLevelSequenceToPlay(ToPlay);
#pragma endregion

#pragma region SetLinks
    SceneNode1->SetNodeID(FGuid::NewGuid());
    SceneNode2->SetNodeID(FGuid::NewGuid());
    SceneNode3->SetNodeID(FGuid::NewGuid());
    SceneNode4->SetNodeID(FGuid::NewGuid());
    EndNode->SetNodeID(FGuid::NewGuid());

    StartNode->SetNextNodeID(SceneNode1->GetNodeID());

    SceneNode1->SetPrevNodeID(StartNode->GetNodeID());
    SceneNode1->SetNextNodeID(SceneNode2->GetNodeID());

    SceneNode2->SetPrevNodeID(SceneNode1->GetNodeID());
    SceneNode2->SetNextNodeID(SceneNode3->GetNodeID());

    SceneNode3->SetPrevNodeID(SceneNode2->GetNodeID());
    SceneNode3->SetNextNodeID(SceneNode4->GetNodeID());

    SceneNode4->SetPrevNodeID(SceneNode3->GetNodeID());
    SceneNode4->SetNextNodeID(EndNode->GetNodeID());

    EndNode->SetPrevNodeID(SceneNode4->GetNodeID());
#pragma endregion

#pragma region AddNodes
    NewDialogueGraph->AddNode(SceneNode1);
    NewDialogueGraph->AddNode(SceneNode2);
    NewDialogueGraph->AddNode(SceneNode3);
    NewDialogueGraph->AddNode(SceneNode4);
    NewDialogueGraph->AddNode(EndNode);
#pragma endregion

#pragma endregion

    StartDialogue(MasterLevelSequence, NewDialogueGraph);
}

void UDialoguePlayerSubsystem::StopDialogue() const
{
    DialogueInstance->Stop();
}

void UDialoguePlayerSubsystem::RemoveDialogue()
{
    DialogueInstance->Rename(nullptr, nullptr);
    DialogueInstance->RemoveFromRoot();
    DialogueInstance = nullptr;
}
