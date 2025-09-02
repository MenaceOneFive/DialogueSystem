// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/DialoguePlayerInstance.h"

#include "AbilitySystemInterface.h"
#include "DialogueSystemRuntimeModule.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlaybackSettings.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Component/DialogueUIComponent.h"
#include "GameFramework/Character.h"
#include "Graph/DialogueGraph.h"
#include "Graph/DialogueGraphDirector.h"
#include "Graph/Node/DialogueCustomNode.h"
#include "Graph/Node/DialogueEndNode.h"
#include "Graph/Node/DialogueGraphNode.h"
#include "Graph/Node/DialogueLineNode.h"
#include "Graph/Node/DialogueSelectionItem.h"
#include "Graph/Node/DialogueSelectionNode.h"
#include "Graph/Node/DialogueStartNode.h"
#include "Kismet/GameplayStatics.h"
#include "Player/LevelSequencePlayerPool.h"
#include "Subsystem/DialoguePlayerSubsystem.h"
#include "Util/SequencerUtil.h"

class UDialogueGraphDirector;


void UDialogueGraphVisitNextDelegateExecutor::Initialize(const FSimpleDelegate& InVisitNextDelegate)
{
    OnVisitNextDelegate = InVisitNextDelegate;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UDialogueGraphVisitNextDelegateExecutor::VisitNextNode()
{
    (void)OnVisitNextDelegate.ExecuteIfBound();
}

void UDialogueRuntimePlayer::PlayFromBeginning()
{
    if (!DialogueGraph)
    {
        UE_LOG(LogTemp, Error, TEXT("%s : DialogueGraph is nullptr"), TEXT(__FUNCTION__));
        return;
    }
    if (const auto StartNode = DialogueGraph->GetStartNode())
    {
        InitializeVisitor();
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
    if (InDialogueGraph)
    {
        this->DialogueGraph = InDialogueGraph;
        InitializePlayers();
    }

    // 블루프린트를 초기화
    const auto Class = InDialogueGraph->GetBlueprintClass();

    check(Class->IsChildOf(UDialogueGraphDirector::StaticClass()))

    DialogueGraphDirector = NewObject<UDialogueGraphDirector>(this, Class, FName("GraphDirectorInstance"), RF_Transient);

    // TODO : 초기화 메서드에서 정확한 인스턴스를 받도록 수정
    if (auto OwningCharacter = GetTypedOuter<ACharacter>())
    {
        DialogueGraphDirector->SetPlayerCharacter(OwningCharacter);
        // 어빌리티 시스템을 구현한 캐릭터라면 ASC에 대한 참조도 저장
        if (OwningCharacter->Implements<UAbilitySystemInterface>())
        {
            TScriptInterface<IAbilitySystemInterface> IASC = OwningCharacter;
            DialogueGraphDirector->SetPlayerASC(IASC->GetAbilitySystemComponent());
        }
    }
}

void UDialogueRuntimePlayer::InitializeVisitor()
{
    const auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    checkf(PlayerController, TEXT("플레이어 컨트롤러를 찾을 수 없습니다."));

    const auto PlayerCharacter = PlayerController ? PlayerController->GetCharacter() : nullptr;
    checkf(PlayerCharacter, TEXT("플레이어 캐릭터를 찾을 수 없습니다."));

    UIComponent = ResolveUIComponent();
    checkf(UIComponent, TEXT("입력 컴포넌트를 찾을 수 없습니다."));

    Module = &FModuleManager::Get().GetModuleChecked<FDialogueSystemRuntimeModule>("DialogueSystemRuntime");

    InitializePlayers();
}

void UDialogueRuntimePlayer::VisitNextNodeFromSelection(const UDialogueGraphNode* CurrentNode, const FGuid NextNodeId)
{
    UIComponent->HideSelectionUI();
    VisitNextNode(CurrentNode, NextNodeId);
}

void UDialogueRuntimePlayer::VisitNextNode(const UDialogueGraphNode* CurrentNode, const FGuid NextNodeId)
{
    PrevNode                   = CurrentNode;
    const auto NextNodeToVisit = DialogueGraph->GetNode(NextNodeId);
    if (NextNodeId == InvalidGuid || !NextNodeToVisit)
    {
        UE_LOG(LogTemp, Error, TEXT("다음 노드가 없습니다. 대화를 종료합니다. NodeID : %s"), *NextNodeId.ToString());
        // 여기에 종료 연산 추가
    }
    NextNodeToVisit->Accept(this);
}

void UDialogueRuntimePlayer::VisitNextNodeFromScene(const ULevelSequencePlayerHolder* InCurrentHolder, const UDialogueGraphNode* InCurrentNode, FGuid InNextNodeId)
{
    ULevelSequencePlayer* SequencePlayer = InCurrentHolder->GetLevelSequencePlayer();
    SequencePlayer->OnFinished.Clear();
    SequencePlayer->OnPause.Clear();
    SequencePlayer->OnStop.Clear();
    SequencePlayer->OnPlay.Clear();
    SequencePlayer->OnNativeFinished.Unbind();
    SequencePlayer->OnPlayReverse.Clear();
    SequencePlayer->OnCameraCut.Clear();
    VisitNextNode(InCurrentNode, InNextNodeId);
}

FSimpleDelegate UDialogueRuntimePlayer::MakeVisitNextNodeFromSelectionDelegate(const UDialogueGraphNode* InCurrentNode, FGuid InNextNodeId)
{
    return FSimpleDelegate::CreateUObject(this, &UDialogueRuntimePlayer::VisitNextNodeFromSelection, InCurrentNode, InNextNodeId);
}

FSimpleDelegate UDialogueRuntimePlayer::MakeVisitNextNodeDelegate(const ULevelSequencePlayerHolder* InCurrentHolder, const UDialogueGraphNode* InCurrentNode, FGuid InNextNodeId)
{
    return FSimpleDelegate::CreateUObject(this, &UDialogueRuntimePlayer::VisitNextNodeFromScene, InCurrentHolder, InCurrentNode, InNextNodeId);
}

void UDialogueRuntimePlayer::VisitStartNode(const TObjectPtr<const UDialogueStartNode>& StartNode)
{
    ApplyDialogueUISetting(StartNode->GetDialogueSetting());
    // 정의되어 있으면 방문 효과를 발생시킨다
    WhenVisitThisNode(PrevNode, StartNode);

    // 시작 노드의 데이터를 이용한 처리
    // ...

    // 다음 노드를 방문처리
    VisitNextNode(StartNode, StartNode->GetNextNodeID());
}

void UDialogueRuntimePlayer::VisitSelectionNode(const TObjectPtr<const UDialogueSelectionNode>& SelectionNode)
{
    ApplyDialogueUISetting(SelectionNode->GetDialogueSetting());
    // 정의되어 있으면 방문 효과를 발생시킨다
    WhenVisitThisNode(PrevNode, SelectionNode);

    // 선택 노드의 데이터를 이용한 처리
    TArray<TObjectPtr<UDialogueSelectionItem>> SelectionItems;
    for (const auto& ItemNodeIds = SelectionNode->GetNextNodeIDs();
         const auto& ItemNodeId : ItemNodeIds)
    {
        auto Node = DialogueGraph->GetNode(ItemNodeId);
        if (!Node)
        {
            continue;
        }
        FText Name          = GetSelectionName(Node);
        auto  SelectionItem = NewObject<UDialogueSelectionItem>(this);
        SelectionItem->Initialize(MakeVisitNextNodeFromSelectionDelegate(SelectionNode, ItemNodeId), CanSelectNode(ItemNodeId));
        SelectionItem->SetSelectionName(Name);
        SelectionItems.Add(SelectionItem);
    }

    UIComponent->SetSelectionItem(SelectionItems);
    UIComponent->ShowSelectionUI();
    FocusOnRootWidget();
    ShowCursor();
    PrevNode = SelectionNode;
    // 선택 노드는 UI에서의 선택에 따라 처리하므로 별도의 방문처리는 수행하지 않는다.
}

void UDialogueRuntimePlayer::VisitSceneNode(const TObjectPtr<const UDialogueLineNode>& SceneNode)
{
    ApplyDialogueUISetting(SceneNode->GetDialogueSetting());
    // 정의되어 있으면 방문 효과를 발생시킨다
    WhenVisitThisNode(PrevNode, SceneNode);

    /////////////////////////////////////////////////////////////////////////////////
    // 장면에 대한 재생처리
    const TObjectPtr<ULevelSequencePlayerHolder> Holder              = SceneNodeToPlayerHolder[SceneNode];
    ULevelSequencePlayer*                        LevelSequencePlayer = Holder->GetLevelSequencePlayer();
    InitializeSequencePlayer(Holder, SceneNode, SceneNode->GetNextNodeID());

    ApplySequencePlaybackSetting(LevelSequencePlayer, SceneNode->GetDialogueSetting(), SceneNode->GetSequencePlaySetting());

    CurrentHolder = Holder;
    PrevNode      = SceneNode;

    LevelSequencePlayer->Play();
}

void UDialogueRuntimePlayer::VisitEndNode(const TObjectPtr<const UDialogueEndNode>& EndNode)
{
    ApplyDialogueUISetting(EndNode->GetDialogueSetting());
    // 정의되어 있으면 방문 효과를 발생시킨다
    WhenVisitThisNode(PrevNode, EndNode);

    // 리소스 정리를 요청
    if (const auto World = GetWorld())
    {
        FTimerHandle TimeHandle;
        World->GetTimerManager().SetTimer(TimeHandle,
                                          this,
                                          &UDialogueRuntimePlayer::HideSubtitle,
                                          3.0f, false);
    }

    CurrentHolder = nullptr;
    NextNode      = nullptr;
    PrevNode      = nullptr;
}

void UDialogueRuntimePlayer::VisitCustomNode(const TObjectPtr<const UDialogueCustomNode>& CustomNode)
{
    ApplyDialogueUISetting(CustomNode->GetDialogueSetting());

    // 정의되어 있으면 방문 효과를 발생시킨다
    WhenVisitThisNode(PrevNode, CustomNode);

    // 커스텀 노드의 데이터를 이용한 처리
    if (const auto Definition = Module->GetCustomNodeManager()->GetDefinitionOfCustomNode(CustomNode->GetClass()))
    {
        // Visitor를 넘기는 것이 맞을까?
        Definition->EvaluateCustomNode(CustomNode, this);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("이 노드를 처리하는 방법을 알 수 없습니다."))
    }

    // 다음 노드를 방문처리
    // 추후 다출력 커스텀 노드가 추가되면 수정이 필요할 수 있음
    VisitNextNode(CustomNode, CustomNode->GetNextNodeID());
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UDialogueRuntimePlayer::InitializePlayers()
{
    const auto                             World        = GetWorld();
    const UGameInstance*                   GameInstance = World->GetGameInstance();
    const UDialoguePlayerRuntimeSubsystem* Subsystem    = GameInstance->GetSubsystem<UDialoguePlayerRuntimeSubsystem>();

    for (const UDialogueGraphNode* DialogueNode : DialogueGraph->GetNodes())
    {
        if (DialogueNode->Implements<USequencePlayCapability>())
        {
            TScriptInterface<const ISequencePlayCapability> SceneNode(DialogueNode);
            const auto                                      LevelSequence  = SceneNode->GetLevelSequenceToPlay();
            const auto                                      LoadedSequence = LevelSequence.LoadSynchronous();
            auto                                            PlayerHolder   = Subsystem->RequestNewPlayerInstance();
            PlayerHolder->Rename(nullptr, this);
            PlayerHolder->SetLevelSequence(LoadedSequence);
            SceneNodeToPlayerHolder.Add(DialogueNode, PlayerHolder);
        }
    }
}

void UDialogueRuntimePlayer::InitializeSequencePlayer(const ULevelSequencePlayerHolder* Holder, const UDialogueGraphNode* CurrentNode, const FGuid NextNodeId)
{
    ULevelSequencePlayer* SequencePlayer = Holder->GetLevelSequencePlayer();
    SequencePlayer->OnStop.Clear();
    SequencePlayer->OnPause.Clear();
    SequencePlayer->OnFinished.Clear();

    // 시퀀스 플레이어의 델리게이트는 입/출력이 없는 다이나믹 델리게이트
    // 근데 다른 노드 방문 처리를 위해서는 방문에 대한 정보가 필요한 상황
    // 그래서 UDialogueGraphVisitNextDelegateExecutor를 정의했고, 이것의 VisitNextNode를 실행할 때 주입한 FSimpleDelegate를 실행하도록 설계함
    UDialogueGraphVisitNextDelegateExecutor* VisitNextExecutor = NewObject<UDialogueGraphVisitNextDelegateExecutor>(this);
    VisitNextExecutor->Initialize(MakeVisitNextNodeDelegate(Holder, CurrentNode, NextNodeId));

    // 새 바인딩을 설정
    SequencePlayer->OnStop.AddDynamic(VisitNextExecutor, &UDialogueGraphVisitNextDelegateExecutor::VisitNextNode);
    SequencePlayer->OnPause.AddDynamic(VisitNextExecutor, &UDialogueGraphVisitNextDelegateExecutor::VisitNextNode);
    SequencePlayer->OnFinished.AddDynamic(VisitNextExecutor, &UDialogueGraphVisitNextDelegateExecutor::VisitNextNode);
}

void UDialogueRuntimePlayer::FocusOnRootWidget() const
{
    const auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

    const auto Component = ResolveUIComponent();
    check(Component);
    if (!Component)
    {
        return;
    }

    const auto RootWidget = Component->GetRootWidget()->GetRootWidget();
    UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PlayerController, RootWidget, EMouseLockMode::LockAlways);
}

void UDialogueRuntimePlayer::UnfocusFromWidget() const
{
    if (const auto World = GetWorld(); World->IsGameWorld())
    {
        if (const auto PlayerController = UGameplayStatics::GetPlayerController(World, 0))
        {
            UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
        }
    }
}

void UDialogueRuntimePlayer::ShowCursor() const
{
    if (const auto World = GetWorld(); World->IsGameWorld())
    {
        if (const auto PlayerController = UGameplayStatics::GetPlayerController(World, 0))
        {
            PlayerController->SetShowMouseCursor(true);
            if (const auto Component = ResolveUIComponent())
            {
                Component->ShowCursor();
            }
        }
    }
}

void UDialogueRuntimePlayer::HideCursor() const
{
    if (const auto World = GetWorld(); World->IsGameWorld())
    {
        if (const auto PlayerController = UGameplayStatics::GetPlayerController(World, 0))
        {
            PlayerController->SetShowMouseCursor(false);
            if (const auto Component = ResolveUIComponent())
            {
                Component->HideCursor();
            }
        }
    }
}

UDialogueUIComponent* UDialogueRuntimePlayer::ResolveUIComponent() const
{
    // 이미 보유 중이면 그대로 반환
    if (UIComponent)
    {
        return UIComponent;
    }

    // 월드/컨트롤러/폰을 통해 컴포넌트를 조회
    if (const auto World = GetWorld(); World && World->IsGameWorld())
    {
        if (const auto PlayerController = UGameplayStatics::GetPlayerController(World, 0))
        {
            if (const auto ControlledPawn = PlayerController->GetPawn())
            {
                return ControlledPawn->FindComponentByClass<UDialogueUIComponent>();
            }
        }
    }
    return nullptr;
}

void UDialogueRuntimePlayer::BindKeyDownEvent(FKey Key, FDialoguePlayerKeyboardEvent InEvent)
{
    if (const auto Component = ResolveUIComponent())
    {
        Component->BindKeyDownEvent(Key, InEvent);
    }
}

void UDialogueRuntimePlayer::BindKeyUpEvent(FKey Key, FDialoguePlayerKeyboardEvent InEvent)
{
    if (const auto Component = ResolveUIComponent())
    {
        Component->BindKeyUpEvent(Key, InEvent);
    }
}

void UDialogueRuntimePlayer::BindMouseButtonDownEvent(FKey MouseButton, FDialoguePlayerUIMouseEvent InEvent)
{
    if (const auto Component = ResolveUIComponent())
    {
        Component->BindMouseButtonDownEvent(MouseButton, InEvent);
    }
}

void UDialogueRuntimePlayer::BindMouseButtonUpEvent(FKey MouseButton, FDialoguePlayerUIMouseEvent InEvent)
{
    if (const auto Component = ResolveUIComponent())
    {
        Component->BindMouseButtonUpEvent(MouseButton, InEvent);
    }
}

void UDialogueRuntimePlayer::BindMouseButtonDoubleClickEvent(FKey MouseButton, FDialoguePlayerUIMouseEvent InEvent)
{
    if (const auto Component = ResolveUIComponent())
    {
        Component->BindMouseButtonDoubleClickEvent(MouseButton, InEvent);
    }
}

void UDialogueRuntimePlayer::UnbindKeyDownEvent(FKey Key)
{
    if (const auto Component = ResolveUIComponent())
    {
        Component->UnbindKeyDownEvent(Key);
    }
}

void UDialogueRuntimePlayer::UnbindKeyUpEvent(FKey Key)
{
    if (const auto Component = ResolveUIComponent())
    {
        Component->UnbindKeyUpEvent(Key);
    }
}

void UDialogueRuntimePlayer::UnbindMouseButtonDownEvent(FKey MouseButton)
{
    if (const auto Component = ResolveUIComponent())
    {
        Component->UnbindMouseButtonDownEvent(MouseButton);
    }
}

void UDialogueRuntimePlayer::UnbindMouseButtonUpEvent(FKey MouseButton)
{
    if (const auto Component = ResolveUIComponent())
    {
        Component->UnbindMouseButtonUpEvent(MouseButton);
    }
}

void UDialogueRuntimePlayer::UnbindMouseButtonDoubleClickEvent(FKey MouseButton)
{
    if (const auto Component = ResolveUIComponent())
    {
        Component->UnbindMouseButtonDoubleClickEvent(MouseButton);
    }
}

void UDialogueRuntimePlayer::ClearAllBindings()
{
    if (const auto Component = ResolveUIComponent())
    {
        Component->ClearAllBindings();
    }
}

FText UDialogueRuntimePlayer::GetSelectionName(const TObjectPtr<const UDialogueGraphNode>& Node)
{
    if (Node->Implements<UCanSelectThisNode>())
    {
        return TScriptInterface<const ICanSelectThisNode>(Node)->GetSelectionName();
    }
    if (Node->IsA<UDialogueEndNode>())
    {
        return NSLOCTEXT("DialoguePlayer", "EndNode", "끝내기");
    }
    UE_LOG(LogTemp, Error, TEXT("선택지 노드에 알 수 없는 유형(%s)의 노드가 연결되어 있습니다."), *Node->GetClass()->GetPathName());
    return FText::FromString("???");
}


void UDialogueRuntimePlayer::ApplyDialogueUISetting(const FDialogueSetting DialogueSetting) const
{
    switch (DialogueSetting.EFocusMode)
    {
        case EFocusMode::UI:
            FocusOnRootWidget();
            break;
        case EFocusMode::Game:
            UnfocusFromWidget();
            break;
    }

    if (DialogueSetting.bShouldShowCursor)
    {
        ShowCursor();
    }
    else
    {
        HideCursor();
    }
}

void UDialogueRuntimePlayer::WhenVisitThisNode(const TObjectPtr<const UDialogueGraphNode>& InPrevNode,
                                               const TObjectPtr<const UDialogueGraphNode>& InCurrentNode) const
{
    // 방문을 통해 발생하는 효과 처리
    const auto DirectorClass = DialogueGraph->GetBlueprintClass();

    if (const auto WhenVisitThisNodeName = InCurrentNode->GetWhenSelectThisNodeFunctionName(); !WhenVisitThisNodeName.IsEqual(NAME_None))
    {
        const auto Function = DirectorClass->FindFunctionByName(WhenVisitThisNodeName);
        checkf(Function, TEXT("에디터에서는 존재했던 함수가 런타임에서는 확인되지 않는 현상입니다."))
        if (!Function)
        {
            return;
        }
        DialogueGraphDirector->WhenVisitThisNode(Function, InPrevNode, InCurrentNode);
    }

    if (DialogueGraphDirector->WasNodeVisited(InCurrentNode))
    {
        UE_LOG(LogTemp, Log, TEXT("이미 방문한 노드입니다."))
    }
    DialogueGraphDirector->MarkNodeAsVisited(InCurrentNode);
}


void UDialogueRuntimePlayer::SkipCurrentNode() const
{
    // Plugins\DialogueSystem\DialogueSystem\Source\DialogueSystemRuntime\Private\Player\DialoguePlayerInstance.cpp:445 에 따르면
    // 현재 재생중인 노드는 PrevNode이다.
    if (PrevNode && PrevNode->Implements<USkippableNode>())
    {
        if (const auto SceneNode = Cast<UDialogueLineNode>(PrevNode); SceneNode && SceneNode->CanSkipThisNode())
        {
            check(IsValid(CurrentHolder))
            const auto CurrentPlayer = CurrentHolder->GetLevelSequencePlayer();
            check(CurrentPlayer)
            const FQualifiedFrameTime         EndTime = CurrentPlayer->GetEndTime();
            FMovieSceneSequencePlaybackParams PlaybackParams;
            PlaybackParams.bHasJumped   = true;
            PlaybackParams.Timecode     = EndTime.ToTimecode();
            PlaybackParams.PositionType = EMovieScenePositionType::Timecode;
            CurrentPlayer->SetPlaybackPosition(PlaybackParams);
        }
    }
}

TObjectPtr<ULevelSequencePlayerHolder> UDialogueRuntimePlayer::GetPlayerHolder(const TObjectPtr<const UDialogueCustomNode>& GraphNode)
{
    if (!SceneNodeToPlayerHolder.Contains(GraphNode))
    {
        return nullptr;
    }

    auto Holder = SceneNodeToPlayerHolder[GraphNode];

    ULevelSequencePlayer* LevelSequencePlayer = Holder->GetLevelSequencePlayer();
    InitializeSequencePlayer(Holder, GraphNode, GraphNode->GetNextNodeID());

    ApplySequencePlaybackSetting(LevelSequencePlayer, GraphNode->GetDialogueSetting(), GraphNode->GetSequencePlaySetting());


    return Holder;
}

void UDialogueRuntimePlayer::PlaySequence(const TObjectPtr<const UDialogueCustomNode>& InCustomNode, const TObjectPtr<ULevelSequencePlayerHolder>& Holder)
{
    CurrentHolder = Holder;
    PrevNode      = InCustomNode;

    Holder->GetLevelSequencePlayer()->Play();
}

bool UDialogueRuntimePlayer::CanSelectNode(const FGuid& NodeGuid) const
{
    bool bCanSelectThisNode = true;
    if (const auto SelectableNode = DialogueGraph->GetNode(NodeGuid);
        SelectableNode &&
        SelectableNode->Implements<UCanSelectThisNode>())
    {
        if (const TScriptInterface<const ICanSelectThisNode> CanSelectThisNodeInterface(SelectableNode);
            CanSelectThisNodeInterface.GetInterface())
        {
            // 조건을 검사
            // 만약 별도로 지정한 Predicate가 없으면 허용으로 간주
            if (const auto FunctionName = CanSelectThisNodeInterface->GetCanSelectThisNodeFunctionName();
                !FunctionName.IsEqual(NAME_None))
            {
                UFunction* CanVisitThisNodeFunction = DialogueGraphDirector->FindFunction(FunctionName);
                checkf(CanVisitThisNodeFunction, TEXT("에디터에서는 존재했던 함수가 런타임에서는 확인되지 않는 현상입니다."))
                if (!CanVisitThisNodeFunction)
                {
                    return false;
                }

                bCanSelectThisNode = DialogueGraphDirector->CanVisitNode(CanVisitThisNodeFunction, SelectableNode);
            }
        }
    }
    return bCanSelectThisNode;
}

void UDialogueRuntimePlayer::HideSubtitle() const
{
    if (const auto Component = ResolveUIComponent())
    {
        Component->HideSubtitle();
    }
}

void UDialogueRuntimePlayer::ApplySequencePlaybackSetting(ULevelSequencePlayer* Player, const FDialogueSetting& DialogueSetting, const FSequencePlaySetting& PlaySetting)
{
    FMovieSceneSequencePlaybackSettings Settings;
    Settings.bAutoPlay                     = false;
    Settings.bPauseAtEnd                   = false;
    Settings.StartTime                     = PlaySetting.StartTimeOfSequence;
    Settings.bDisableMovementInput         = PlaySetting.bShouldBlockPlayerMovementDuringDialogue;
    Settings.bDisableLookAtInput           = PlaySetting.bShouldBlockPlayerCameraMovementDuringDialogue;
    Settings.FinishCompletionStateOverride = PlaySetting.StateAfterSequencePlay == EStateAfterSequencePlay::Keep ? EMovieSceneCompletionModeOverride::ForceKeepState : EMovieSceneCompletionModeOverride::ForceRestoreState;
    Player->SetCompletionModeOverride(PlaySetting.StateAfterSequencePlay == EStateAfterSequencePlay::Keep ? EMovieSceneCompletionModeOverride::ForceKeepState : EMovieSceneCompletionModeOverride::ForceRestoreState);
    Player->SetPlaybackSettings(Settings);
    // 다시 재생할 수 있으니 재생위치 초기화
    Player->SetPlaybackPosition({PlaySetting.StartTimeOfSequence, EUpdatePositionMethod::Jump/** 재생모드가 확실하지 않아서 실험 필요 */});
}
