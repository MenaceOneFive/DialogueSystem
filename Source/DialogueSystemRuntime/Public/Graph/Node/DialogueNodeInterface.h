#pragma once
#include "UObject/Interface.h"
#include "DialogueNodeInterface.generated.h"

DECLARE_DELEGATE_RetVal(bool, FCanSelectThisItem);

/// 디렉터 블루프린트에서 구현한 "선택 가능", "선택 시"에 대한 참조를 담는 인터페이스들
/// 1. 요소
///     CanSelectThisNodeFunctionName  : 이 노드를 선택할 수 있는지 판별하는 메서드의 이름
///     WhenSelectThisNodeFunctionName : 이 노드를 실행하면 어떤 효과를 줄 것인지 정하는 메서드 이름
///
/// 2. 함수 시그니처
///     bool CanSelectThisNode(UDialogueGraphNode* PrevNode, UDialogueGraphNode* ThisNode)
///     void WhenSelectThisNode(UDialogueGraphNode* PrevNode, UDialogueGraphNode* ThisNode)
/// 
/// 3. 상태
///     CanSelectThisNodeFunctionName
///         -1 값이 Name_None : 어떤 경우에서도 선택이 가능하다.
///         -2 값이 Name_None이 아님:
///             -가  FindFunctionByName이 nullptr인 경우 : assert(크래시)
///             -나 FindFunctionByName이 nullptr이 아닌 경우 :
///                 -1) true: 선택가능
///                 -2) false: 선택 불가능
///         
///     WhenSelectThisNodeFunctionName
///         -1 값이 Name_None : 실행하지 않고 노드를 선택한다.
///         -2 값이 Name_None이 아님 :
///             -가 FindFunctionByName이 nullptr인 경우 : assert(크래시)
///             -나 FindFunctionByName이 nullptr이 아닌 경우 : 실행
///     
/// 3. 작동원리
///     빌드시 노드가 CanSelectThisNodeFunctionName, WhenSelectThisNodeFunctionName가 Name_None이 아니라면
///     Blueprint를 인스턴스화 하고 다음의 조회 행위를 수행한다.
///     UFunction* Function = 
///     BlueprintInstance->FindFunctionByName( CanSelectThisNodeFunctionName(또는 WhenSelectThisNodeFunctionName) );
///
///     결과가 nullptr이라면 assert를 이용해서 버그 픽스를 수행한다.
///     nullptr이 아니라면 런타임 노드에 적용
UINTERFACE()
class DIALOGUESYSTEMRUNTIME_API UWhenSelectThisNode : public UInterface
{
    GENERATED_BODY()
};

class DIALOGUESYSTEMRUNTIME_API IWhenSelectThisNode
{
    GENERATED_BODY()

public: // 노드 선택 이벤트 관련 메서드
    virtual FName GetWhenSelectThisNodeFunctionName() const = 0;

#if WITH_EDITORONLY_DATA
    virtual void SetWhenSelectThisNodeFunctionName(FName InWhenSelectThisNodeFunctionName = NAME_None) = 0;
#endif
};

UINTERFACE()
class DIALOGUESYSTEMRUNTIME_API UCanSelectThisNode : public UInterface
{
    GENERATED_BODY()
};

class DIALOGUESYSTEMRUNTIME_API ICanSelectThisNode
{
    GENERATED_BODY()

public: // 노드 선택 이벤트 관련 메서드
    virtual FName GetCanSelectThisNodeFunctionName() const = 0;
    virtual FText GetSelectionName() const = 0;
    virtual FText GetDefaultSelectionName() const = 0;

#if WITH_EDITORONLY_DATA
    virtual void SetCanSelectThisNodeFunctionName(FName InCanSelectThisNodeFunctionName = NAME_None) = 0;
    virtual void SetSelectionName(FText& InSelectionName) = 0;
#endif
};
