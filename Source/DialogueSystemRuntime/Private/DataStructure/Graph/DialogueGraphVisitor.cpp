// Fill out your copyright notice in the Description page of Project Settings.
#include "DataStructure/Graph/DialogueGraphVisitor.h"

#include "MovieSceneSequencePlayer.h"
#include "DataStructure/Graph/DialogueGraph.h"
#include "DataStructure/Graph/DialogueNode.h"
#include "Player/DialoguePlayerInstance.h"


void UDialogueGraphVisitorInstance::Initialize()
{
    UDialoguePlayerInstance* DialogueInstance = GetTypedOuter<UDialoguePlayerInstance>();

    check(DialogueInstance) //DialogueInstance가 null인 경우 NewObject<UDialoguePlayerInstance>()를 호출할 때 Outer를 설정하지 않은 것

    check(DialogueInstance->Implements<USectionStack>())
    SectionStack = TScriptInterface<ISectionStack>(DialogueInstance);

    check(DialogueInstance->Implements<UDialoguePlayer>())
    DialoguePlayer = TScriptInterface<IDialoguePlayer>(DialogueInstance);

    check(DialogueInstance->Implements<UDialoguePositionController>())
    DialoguePositionController = TScriptInterface<IDialoguePositionController>(DialogueInstance);

    bIsInitialized = true;
}

void UDialogueGraphVisitorInstance::VisitStartNode(UDialogueStartNode* StartNode)
{
    check(bIsInitialized)
    VisitNextNode(StartNode);
}

void UDialogueGraphVisitorInstance::VisitSelectionNode(UDialogueSelectionNode* SelectionNode)
{
    UDialogueGraph* ParentGraph = SelectionNode->GetTypedOuter<UDialogueGraph>();
    check(bIsInitialized && ParentGraph)
    // TODO: 뒤로가기 구현 -> 틀렸음 (가설)에디터에서 출력을 이전 노드로 지정하면 뒤로 가는 효과가 있을 것
    // UDialogueUISubsystem에 항목을 등록
    for (FDialogueSelectionItem& SelectionItem : SelectionNode->GetSelectionItems())
    {
        const auto OnItemSelected = [this, ParentGraph, SelectionItem]()
        {
            // ParentGraph 안에서 Assert가 수행되므로 사전 / 사후 유효성을 확인할 필요 없음
            UDialogueGraphNode* NextNode = ParentGraph->GetNode(SelectionItem.NodeID);

            // TODO: 여기서 UDialogueUISubsystem에서 항목을 제거
            //

            DialoguePositionController->SetCurrentDialoguePosition(NextNode);
            NextNode->VisitNode(this); // 다음 노드가 뭔지는 알 필요는 없음
        };
    }
    // Play하지 말 것, Pause는 절대 하지 말 것
}

void UDialogueGraphVisitorInstance::VisitSceneNode(UDialogueSceneNode* SceneNode)
{
    // TODO: SceneNode에서 ULevelSequence를 받아야 함
    ULevelSequence* SequenceToPlay = SceneNode->GetLevelSequenceToPlay();
    check(bIsInitialized && SequenceToPlay)

    FMovieSceneSequencePlaybackParams PlaybackParams;
    FMovieSceneSequencePlayToParams PlayToParams;
    SectionStack->PushShotSection(SequenceToPlay, PlaybackParams, PlayToParams);
    DialoguePlayer->PlayTo(PlaybackParams, PlayToParams);

    UDialogueGraph* ParentGraph = SceneNode->GetTypedOuter<UDialogueGraph>();
    check(ParentGraph);
    UDialogueGraphNode* NextNode = ParentGraph->GetNode(SceneNode->GetNextNodeID());
    check(NextNode);

    DialoguePositionController->SetCurrentDialoguePosition(NextNode);
}

void UDialogueGraphVisitorInstance::VisitEndNode(UDialogueEndNode* EndNode)
{
    check(bIsInitialized)
    DialoguePlayer->Stop();
}

void UDialogueGraphVisitorInstance::VisitEventNode(UDialogueEventNode* ScriptNode)
{
    check(bIsInitialized)
    // TODO: 스크립트 실행하는 로직 추가

    VisitNextNode(ScriptNode);
}

void UDialogueGraphVisitorInstance::VisitNextNode(const UDialogueGraphNode* GraphNode)
{
    UDialogueGraph* ParentGraph = GraphNode->GetTypedOuter<UDialogueGraph>();
    check(bIsInitialized && ParentGraph)

    // ParentGraph 안에서 Assert가 수행되므로 사전 / 사후 유효성을 확인할 필요 없음
    UDialogueGraphNode* NextNode = ParentGraph->GetNode(GraphNode->GetNextNodeID());

    TScriptInterface<IDialoguePositionController>::InterfaceType* PositionController = DialoguePositionController.GetInterface();
    PositionController->GetCurrentDialoguePosition();
    PositionController->SetCurrentDialoguePosition(NextNode);
    NextNode->VisitNode(this); // 다음 노드가 어떤 유형인지는 알 필요는 없음
}
