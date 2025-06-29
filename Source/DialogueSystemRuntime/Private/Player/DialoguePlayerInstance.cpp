// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/DialoguePlayerInstance.h"

#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Component/DialogueUIComponent.h"
#include "GameFramework/Character.h"
#include "Graph/DialogueGraph.h"
#include "Graph/DialogueGraphDirector.h"
#include "Graph/Node/DialogueEndNode.h"
#include "Graph/Node/DialogueEventNode.h"
#include "Graph/Node/DialogueGraphNode.h"
#include "Graph/Node/DialogueSceneNode.h"
#include "Graph/Node/DialogueSelectionItem.h"
#include "Graph/Node/DialogueSelectionNode.h"
#include "Graph/Node/DialogueStartNode.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/DialoguePlayerSubsystem.h"
#include "Util/SequencerUtil.h"

class UDialogueGraphDirector;

void UDialoguePlayerInstancePool::InitializeSequencePlayerPool(const uint32 NumberOfInitialPlayerInstance)
{
    // 빈 Dummy LevelSequence를 생성 : CreateLevelSequencePlayer에 필요해서
    EmptyLevelSequence             = NewObject<ULevelSequence>(this);
    EmptyLevelSequence->MovieScene = NewObject<UMovieScene>(EmptyLevelSequence);
    EmptyLevelSequence->MovieScene->SetPlaybackRange(0, 1, true);

    // 지정된 수 만큼 Actor들을 생성
    // for ( uint32 i = 0; i < NumberOfInitialPlayerInstance; ++i )
    // {
    //     CreateLevelSequencePlayerActorInstance();
    // }
}

ULevelSequencePlayerHolder* UDialoguePlayerInstancePool::RequestNewPlayerInstance()
{
    // 풀에 남아 있는 플레이어가 없으면 새로 생성
    if ( SequenceActorPool.IsEmpty() )
    {
        CreateLevelSequencePlayerActorInstance();
        check(!SequenceActorPool.IsEmpty())
    }

    ALevelSequenceActor* OutPlayer = *SequenceActorPool.Peek();
    SequenceActorPool.Pop();

    check(this->IsValidLowLevel())

    check(OutPlayer->IsValidLowLevel())

    // Holder를 초기화
    const auto Holder = NewObject<ULevelSequencePlayerHolder>(this);
    Holder->Initialize(this);
    Holder->SetLevelSequencePlayerActor(OutPlayer);

    return Holder;
}

void UDialoguePlayerInstancePool::CreateLevelSequencePlayerActorInstance()
{
    const FMovieSceneSequencePlaybackSettings DefaultSettings = GetDefaultPlaybackSetting();
    ALevelSequenceActor* LevelSequenceActor;
    const auto World = GetWorld();

    checkf(World->IsGameWorld(),
           TEXT("게임 월드가 아닌 곳에서 이 메서드를 호출했고 실제로 이 액터에 접근하면 크래시가 발생합니다."))

    ULevelSequencePlayer::CreateLevelSequencePlayer(World
                                                    , EmptyLevelSequence
                                                    , DefaultSettings
                                                    , LevelSequenceActor);

    if ( LevelSequenceActor )
    {
        // LevelSequenceActor->Rename(nullptr, World);
        // 초기화에 성공했으면 Pool에 삽입
        SequenceActorPool.Enqueue(LevelSequenceActor);
    }
}

void UDialoguePlayerInstancePool::ReturnSequencePlayerActorInstance(ALevelSequenceActor* SequenceActorToReturn)
{
    if ( const auto SequencePlayer = SequenceActorToReturn->GetSequencePlayer() )
    {
        SequencePlayer->Stop();
    }
    SequenceActorPool.Enqueue(SequenceActorToReturn);
}

FMovieSceneSequencePlaybackSettings UDialoguePlayerInstancePool::GetDefaultPlaybackSetting() const
{
    FMovieSceneSequencePlaybackSettings DefaultSettings;
    DefaultSettings.bAutoPlay   = false;
    DefaultSettings.bPauseAtEnd = true;
    return DefaultSettings;
}

void ULevelSequencePlayerHolder::Initialize(UDialoguePlayerInstancePool* PlayerPool)
{
    ParentPool = PlayerPool;
}

void ULevelSequencePlayerHolder::BeginDestroy()
{
    UObject::BeginDestroy();

    // 이 홀더에 할당된 자원을 반납
    if ( SequencePlayerActor && SequencePlayerActor->IsValidLowLevel() )
    {
        ParentPool->ReturnSequencePlayerActorInstance(SequencePlayerActor);
    }

    // 연결 끊기
    ParentPool          = nullptr;
    SequencePlayerActor = nullptr;
}

void ULevelSequencePlayerHolder::SetLevelSequencePlayerActor(ALevelSequenceActor* InPlayerActor)
{
    if ( SequencePlayerActor && SequencePlayerActor->IsValidLowLevel() )
    {
    }
    SequencePlayerActor = InPlayerActor;
}

ULevelSequencePlayer* ULevelSequencePlayerHolder::GetLevelSequencePlayer() const
{
    if ( SequencePlayerActor )
    {
        if ( const auto Player = SequencePlayerActor->GetSequencePlayer() )
        {
            return Player;
        }
    }
    return nullptr;
}

void ULevelSequencePlayerHolder::SetLevelSequence(ULevelSequence* LevelSequenceToPlay) const
{
    // IsPlaying을 false로 만들기
    auto LevelSequencePlayer = GetLevelSequencePlayer();
    LevelSequencePlayer->Pause();
    SequencePlayerActor->SetSequence(LevelSequenceToPlay);
}

void UDialogueRuntimePlayer::PlayFromBeginning()
{
    if ( !DialogueGraph )
    {
        UE_LOG(LogTemp, Error, TEXT("%s : DialogueGraph is nullptr"), TEXT(__FUNCTION__));
        return;
    }
    if ( const auto StartNode = DialogueGraph->GetStartNode() )
    {
        StartNode->Accept(this);
        return;
    }
    UE_LOG(LogTemp, Error, TEXT("%s : StartNode is nullptr."), TEXT(__FUNCTION__));
}

void UDialogueRuntimePlayer::Pause()
{
}

void UDialogueRuntimePlayer::Stop()
{
}

void UDialogueRuntimePlayer::SetDialogueGraph(const UDialogueGraph* InDialogueGraph)
{
    if ( InDialogueGraph )
    {
        this->DialogueGraph = InDialogueGraph;
        InitializePlayers();
    }

    const auto Class = DialogueGraph->GetBlueprintClass();

    check(Class->IsChildOf(UDialogueGraphDirector::StaticClass()))

    DialogueGraphDirector = NewObject<UDialogueGraphDirector>(this, Class, FName("GraphDirectorInstance"), RF_Transient);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UDialogueRuntimePlayer::VisitNextNode()
{
    // 등록된 콜백들을 모두 제거
    check(CurrentHolder)
    {
        CurrentHolder->GetLevelSequencePlayer()->OnStop.Clear();
        CurrentHolder->GetLevelSequencePlayer()->OnPause.Clear();
        CurrentHolder->GetLevelSequencePlayer()->OnFinished.Clear();
    }

    check(NextNode)
    {
        NextNode->Accept(this);
    }
}

void UDialogueRuntimePlayer::InitializePlayers()
{
    const auto World                                 = GetWorld();
    const UGameInstance* GameInstance                = World->GetGameInstance();
    const UDialoguePlayerRuntimeSubsystem* Subsystem = GameInstance->GetSubsystem<UDialoguePlayerRuntimeSubsystem>();

    for ( const UDialogueGraphNode* DialogueNode : DialogueGraph->GetNodes() )
    {
        if ( const UDialogueSceneNode* SceneNode = Cast<const UDialogueSceneNode>(DialogueNode) )
        {
            const auto LevelSequence = SceneNode->GetLevelSequenceToPlay();
            auto PlayerHolder        = Subsystem->RequestNewPlayerInstance();
            PlayerHolder->Rename(nullptr, this);
            PlayerHolder->SetLevelSequence(LevelSequence);
            SceneNodeToPlayerHolder.Add(SceneNode, PlayerHolder);
        }
    }
}

void UDialogueRuntimePlayer::FocusOnRootWidget() const
{
    const auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    const auto PlayerCharacter  = PlayerController ? PlayerController->GetCharacter() : nullptr;
    const auto Component        = PlayerCharacter ? PlayerCharacter->GetComponentByClass<UDialogueUIComponent>() : nullptr;

    if ( !Component )
    {
        return;
    }

    const auto UIComponent = Cast<UDialogueUIComponent>(Component);
    check(UIComponent)

    const auto RootWidget = UIComponent->GetRootWidget()->GetRootWidget();
    UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PlayerController, RootWidget, EMouseLockMode::LockAlways);
}

void UDialogueRuntimePlayer::UnfocusFromWidget() const
{
    if ( const auto World = GetWorld(); World->IsGameWorld() )
    {
        if ( const auto PlayerController = UGameplayStatics::GetPlayerController(World, 0) )
        {
            UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
        }
    }
}

void UDialogueRuntimePlayer::ShowCursor() const
{
    if ( const auto World = GetWorld(); World->IsGameWorld() )
    {
        if ( const auto PlayerController = UGameplayStatics::GetPlayerController(World, 0) )
        {
            PlayerController->SetShowMouseCursor(true);
            const auto ControlledPawn = PlayerController->GetPawn();
            if ( const auto Component = ControlledPawn->GetComponentByClass<UDialogueUIComponent>() )
            {
                Component->ShowCursor();
            }
        }
    }
}

void UDialogueRuntimePlayer::HideCursor() const
{
    if ( const auto World = GetWorld(); World->IsGameWorld() )
    {
        if ( const auto PlayerController = UGameplayStatics::GetPlayerController(World, 0) )
        {
            PlayerController->SetShowMouseCursor(false);
            const auto ControlledPawn = PlayerController->GetPawn();
            if ( auto Component = ControlledPawn->GetComponentByClass<UDialogueUIComponent>() )
            {
                Component->HideCursor();
            }
        }
    }
}

void UDialogueRuntimePlayer::InitializeVisitor()
{
    NextNode      = DialogueGraph->GetStartNode();
    CurrentHolder = nullptr;
}

void UDialogueRuntimePlayer::VisitStartNode(const TObjectPtr<const UDialogueStartNode>& StartNode)
{
    FocusOnRootWidget();
    HideCursor();

    // 방문을 통해 발생하는 효과 처리
    WhenVisitThisNode(PrevNode, StartNode);

    // 다음 노드 설정
    NextNode = DialogueGraph->GetNode(StartNode->GetNextNodeID());

    // 이전 노드 설정
    PrevNode = StartNode;

    NextNode->Accept(this);
}

void UDialogueRuntimePlayer::VisitSelectionNode(const TObjectPtr<const UDialogueSelectionNode>& SelectionNode)
{
    // 방문을 통해 발생하는 효과 처리
    WhenVisitThisNode(PrevNode, SelectionNode);

    // 선택지 초기화
    const auto NextNodeIds      = SelectionNode->GetNextNodeIDs();
    const auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    const auto PlayerCharacter  = PlayerController ? PlayerController->GetCharacter() : nullptr;
    const auto Component        = PlayerCharacter ? PlayerCharacter->GetComponentByClass<UDialogueUIComponent>() : nullptr;

    if ( !Component )
    {
        return;
    }

    const auto UIComponent = Cast<UDialogueUIComponent>(Component);
    check(UIComponent)

    PrevNode = SelectionNode;

    TArray<UDialogueSelectionItem*> SelectionItem = SelectionNode->GetSelectionItems();
    for ( UDialogueSelectionItem* DialogueSelectionItem : SelectionItem )
    {
        if ( !DialogueSelectionItem->IsValidLowLevel() )
        {
            continue;
        }
        DialogueSelectionItem->InitializeForExecution(DialogueGraphDirector, this);
    }

    UIComponent->SetSelectionItem(SelectionItem);
    UIComponent->ShowSelectionUI();
    FocusOnRootWidget();
    ShowCursor();
}

void UDialogueRuntimePlayer::VisitSceneNode(const TObjectPtr<const UDialogueSceneNode>& SceneNode)
{
    // 방문을 통해 발생하는 효과 처리
    WhenVisitThisNode(PrevNode, SceneNode);

    // 현재 노드를 이용해서 재생을 준비
    HideCursor();
    CurrentHolder = SceneNodeToPlayerHolder[SceneNode];

    NextNode = DialogueGraph->GetNode(SceneNode->GetNextNodeID());

    CurrentHolder->GetLevelSequencePlayer()->OnStop.AddDynamic(this, &UDialogueRuntimePlayer::VisitNextNode);
    CurrentHolder->GetLevelSequencePlayer()->OnPause.AddDynamic(this, &UDialogueRuntimePlayer::VisitNextNode);
    CurrentHolder->GetLevelSequencePlayer()->OnFinished.AddDynamic(this, &UDialogueRuntimePlayer::VisitNextNode);

    const auto Player = CurrentHolder->GetLevelSequencePlayer();

    PrevNode = SceneNode;

    // 다시 재생할 수 있으니 재생위치 초기화
    Player->SetPlaybackPosition({0, EUpdatePositionMethod::Jump/** 재생모드가 확실하지 않아서 실험 필요 */});
    Player->Play();
    HideCursor();
}

void UDialogueRuntimePlayer::VisitEndNode(const TObjectPtr<const UDialogueEndNode>& EndNode)
{
    // 방문을 통해 발생하는 효과 처리
    WhenVisitThisNode(PrevNode, EndNode);

    // 리소스 정리
    UnfocusFromWidget();
    HideCursor();

    TArray<TObjectPtr<ULevelSequencePlayerHolder>> OutArray;
    SceneNodeToPlayerHolder.GenerateValueArray(OutArray);
    for ( const auto LevelSequencePlayerHolder : OutArray )
    {
        const auto Player = LevelSequencePlayerHolder->GetLevelSequencePlayer();
        Player->OnStop.Clear();
        Player->OnPause.Clear();
        Player->OnFinished.Clear();
        Player->Stop();
        CurrentHolder = nullptr;
        NextNode      = nullptr;
        PrevNode      = nullptr;
    }

    if ( const auto World = GetWorld() )
    {
        FTimerHandle TimeHandle;
        World->GetTimerManager().SetTimer(TimeHandle,
                                          this,
                                          &UDialogueRuntimePlayer::HideSubtitle,
                                          3.0f, false
                                         );
    }
}

void UDialogueRuntimePlayer::VisitEventNode(const TObjectPtr<const UDialogueEventNode>& EventNode)
{
    // 방문을 통해 발생하는 효과 처리
    WhenVisitThisNode(PrevNode, EventNode.Get());

    HideCursor();

    NextNode = DialogueGraph->GetNode(EventNode->GetNextNodeID());
    PrevNode = EventNode;

    NextNode->Accept(this);
}

void UDialogueRuntimePlayer::WhenVisitThisNode(const TObjectPtr<const UDialogueGraphNode>& InPrevNode,
                                               const TObjectPtr<const UDialogueGraphNode>& InCurrentNode) const
{
    // 방문을 통해 발생하는 효과 처리
    const auto DirectorClass = DialogueGraph->GetBlueprintClass();

    if ( const auto WhenVisitThisNodeName = InCurrentNode->GetWhenSelectThisNodeFunctionName(); !WhenVisitThisNodeName.IsEqual(NAME_None) )
    {
        const auto Function = DirectorClass->FindFunctionByName(WhenVisitThisNodeName);
        DialogueGraphDirector->WhenVisitThisNode(Function, InPrevNode, InCurrentNode);
    }
}

void UDialogueRuntimePlayer::SkipCurrentNode() const
{
    // Plugins\DialogueSystem\DialogueSystem\Source\DialogueSystemRuntime\Private\Player\DialoguePlayerInstance.cpp:445 에 따르면
    // 현재 재생중인 노드는 PrevNode이다.
    if ( PrevNode && PrevNode->Implements<USkippableNode>() )
    {
        if ( const auto SceneNode = Cast<UDialogueSceneNode>(PrevNode); SceneNode && SceneNode->CanSkipThisNode() )
        {
            check(IsValid(CurrentHolder))
            const auto CurrentPlayer = CurrentHolder->GetLevelSequencePlayer();
            check(CurrentPlayer)
            const FQualifiedFrameTime EndTime = CurrentPlayer->GetEndTime();
            FMovieSceneSequencePlaybackParams PlaybackParams;
            PlaybackParams.bHasJumped   = true;
            PlaybackParams.Timecode     = EndTime.ToTimecode();
            PlaybackParams.PositionType = EMovieScenePositionType::Timecode;
            CurrentPlayer->SetPlaybackPosition(PlaybackParams);
        }
    }
}

void UDialogueRuntimePlayer::HideSubtitle() const
{
    const auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    const auto PlayerCharacter  = PlayerController ? PlayerController->GetCharacter() : nullptr;
    const auto Component        = PlayerCharacter ? PlayerCharacter->GetComponentByClass<UDialogueUIComponent>() : nullptr;

    if ( !Component )
    {
        return;
    }

    const auto UIComponent = Cast<UDialogueUIComponent>(Component);
    check(UIComponent)
    UIComponent->HideSubtitle();
}
