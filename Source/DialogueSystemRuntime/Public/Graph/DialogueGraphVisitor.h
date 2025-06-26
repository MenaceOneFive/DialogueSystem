#pragma once
#include "DialogueGraphVisitor.generated.h"

class IDialoguePositionController;
class UDialogueEventNode;
class UDialogueEndNode;
class UDialogueSelectionNode;
class UDialogueGraphNode;
class UDialoguePlayerInstance;
class IDialoguePlayer;
class ISectionStack;
class UDialogueStartNode;
class UDialogueSceneNode;

/**
 * 
 */
UINTERFACE(BlueprintType)
class URuntimeDialogueGraphVisitor : public UInterface
{
    GENERATED_BODY()
};

/// <summary>
/// IDialogueGraphVisitor 인터페이스
///
/// 이 인터페이스는 대화 시스템에서 Visitor 디자인 패턴을 구현하는 핵심 요소입니다.
/// 다이얼로그 그래프의 각 노드 타입을 방문하고 처리하는 방법을 정의합니다.
/// 
/// Visitor 패턴을 사용하여 다이얼로그 그래프 노드의 구조와 각 노드에 대한 처리 로직을 분리합니다.
/// 이를 통해 새로운 노드 타입이 추가되더라도 기존 코드를 변경하지 않고 처리 로직만 확장할 수 있습니다.
/// 
/// 대화 시스템의 주요 노드 타입:
/// - 시작 노드: 대화의 시작점
/// - 선택 노드: 사용자가 선택할 수 있는 옵션을 제공
/// - 씬 노드: LevelSequence를 재생
/// - 이벤트 노드: 특정 이벤트를 실행
/// - 종료 노드: 대화를 종료
/// 
/// 이 인터페이스 구현체는 다이얼로그 플레이어 인스턴스에서 사용되며, 
/// 그래프 노드 순회 및 시퀀스 재생을 관리합니다.
/// </summary>
class IRuntimeDialogueGraphVisitor
{
    GENERATED_BODY()

public:
    /// <summary>
    /// Visitor가 노드를 처리하기 전에 필수적으로 초기화해야 하는 메서드
    /// 
    /// 이 메서드는 Visitor가 노드를 방문하기 전에 필요한 초기 설정을 수행합니다.
    /// 일반적으로 다이얼로그 플레이어 인스턴스 및 관련 인터페이스에 대한 참조를 설정합니다.
    /// - SectionStack: 시퀀스 섹션 관리
    /// - DialoguePlayer: 시퀀스 재생 제어
    /// - DialoguePositionController: 대화 노드 위치 관리
    /// </summary>
    virtual void InitializeVisitor() = 0;

    /// <summary>
    /// 시작 노드를 방문하고 처리하는 메서드
    /// 
    /// 대화의 시작점을 처리하며, 일반적으로 다음 노드로 이동하는 로직을 수행합니다.
    /// </summary>
    /// <param name="StartNode">처리할 시작 노드 인스턴스</param>
    virtual void VisitStartNode(const UDialogueStartNode* StartNode) = 0;

    /// <summary>
    /// 선택 노드를 방문하고 처리하는 메서드
    /// 
    /// 사용자에게 선택지를 제공하고, 선택에 따라 다른 노드로 분기하는 로직을 수행합니다.
    /// 각 선택지에 대한 콜백을 설정하여 사용자 입력에 따라 다음 노드를 결정합니다.
    /// </summary>
    /// <param name="SelectionNode">처리할 선택 노드 인스턴스</param>
    virtual void VisitSelectionNode(const UDialogueSelectionNode* SelectionNode) = 0;

    /// <summary>
    /// 씬 노드를 방문하고 처리하는 메서드
    /// 
    /// LevelSequence를 재생하는 로직을 수행합니다.
    /// 노드에 연결된 LevelSequence를 가져와 SectionStack에 추가하고 재생합니다.
    /// </summary>
    /// <param name="SceneNode">처리할 씬 노드 인스턴스</param>
    virtual void VisitSceneNode(const UDialogueSceneNode* SceneNode) = 0;

    /// <summary>
    /// 종료 노드를 방문하고 처리하는 메서드
    /// 
    /// 대화를 종료하는 로직을 수행합니다.
    /// 일반적으로 DialoguePlayer의 Stop 메서드를 호출하여 시퀀스 재생을 중지합니다.
    /// </summary>
    /// <param name="EndNode">처리할 종료 노드 인스턴스</param>
    virtual void VisitEndNode(const UDialogueEndNode* EndNode) = 0;

    /// <summary>
    /// 이벤트 노드를 방문하고 처리하는 메서드
    /// 
    /// 특정 이벤트나 스크립트를 실행하는 로직을 수행합니다.
    /// 이벤트 실행 후 일반적으로 다음 노드로 이동합니다.
    /// </summary>
    /// <param name="ScriptNode">처리할 이벤트 노드 인스턴스</param>
    virtual void VisitEventNode(const UDialogueEventNode* ScriptNode) = 0;
};

//
// UCLASS()
// class UDialogueGraphVisitorInstance : public UObject,
//                                       public IRuntimeDialogueGraphVisitor
// {
//     GENERATED_BODY()
//
// #pragma region IDialogueGraphNode_Implementation_Public
//
// public:
//     virtual void InitializeVisitor() override;
//     virtual void VisitStartNode(const UDialogueStartNode* StartNode) override;
//     virtual void VisitSelectionNode(const UDialogueSelectionNode* SelectionNode) override;
//     virtual void VisitSceneNode(const UDialogueSceneNode* SceneNode) override;
//     virtual void VisitEndNode(const UDialogueEndNode* EndNode) override;
//     virtual void VisitEventNode(const UDialogueEventNode* ScriptNode) override;
// #pragma endregion
//
// public:
//     virtual void BeginDestroy() override
//     {
//         SectionStack               = nullptr;
//         DialoguePlayer             = nullptr;
//         DialoguePositionController = nullptr;
//         UE_LOG(LogMemory, Display, TEXT("UDialogueGraphVisitor"))
//         Super::BeginDestroy();
//     };
//
// private:
//
// private:
//     /// <summary>Visitor가 초기화되었는지 여부를 나타내는 플래그</summary>
//     bool bIsInitialized = false;
//
//     /// <summary>섹션 스택 인터페이스에 대한 참조</summary>
//     UPROPERTY()
//     TScriptInterface<ISectionStack> SectionStack;
//
//     /// <summary>대화 플레이어 인터페이스에 대한 참조</summary>
//     UPROPERTY()
//     TScriptInterface<IDialoguePlayer> DialoguePlayer;
//
//     /// <summary>대화 위치 컨트롤러 인터페이스에 대한 참조</summary>
//     UPROPERTY()
//     TScriptInterface<IDialoguePositionController> DialoguePositionController;
// };
//
