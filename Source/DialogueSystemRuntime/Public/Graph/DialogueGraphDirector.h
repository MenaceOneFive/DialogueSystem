// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DialogueGraphDirector.generated.h"

class UDialogueGraphNode;

UCLASS()
class DIALOGUESYSTEMRUNTIME_API UDialogueGraphDirector : public UObject
{
    GENERATED_BODY()

public:
    virtual bool CanVisitNode(const TObjectPtr<UFunction>& Function,
                              const UDialogueGraphNode* NextNodeToVisit);

    virtual void WhenVisitThisNode(const TObjectPtr<UFunction>& Function,
                                   const UDialogueGraphNode* PrevNode,
                                   const UDialogueGraphNode* CurrentNode);

#if WITH_EDITOR

public: // 이 메서드를 호출하지 마십시오 에디터 타임에 시그니처 제공용으로만 쓰입니다.
    UFUNCTION()
    static bool CanSelectThisNodeSignature(const UDialogueGraphNode* ThisNode)
    {
        ensureMsgf(false, TEXT("이 메서드를 호출하지 마십시오 에디터 타임에 시그니처 제공용으로만 쓰입니다."));
        return false;
    }

    UFUNCTION()
    static void WhenSelectThisNodeSignature(const UDialogueGraphNode* PrevNode,
                                            const UDialogueGraphNode* ThisNode)
    {
        ensureMsgf(false, TEXT("이 메서드를 호출하지 마십시오 에디터 타임에 시그니처 제공용으로만 쓰입니다."));
    }

    static FName CanSelectThisNodeSignatureName;
    static FName WhenSelectThisNodeSignatureName;
#endif

private:
    /// <summary>
    /// 블루프린트의 메서드를 안정적으로 호출하기 위해 템플릿으로 작성된 메서드
    /// <br/>
    /// StaticClass()나 GetClass()의 FindFunctionByName를 이용해서 찾은 메서드를 실행할 수 있습니다.
    /// </summary>
    /// <param name="Function">이 인스턴스에서 실행할 수 있는 블루프린트 함수</param>
    /// <param name="OutputValue"></param>
    /// <param name="InInputValue">입력으로 사용할 값</param>
    /// <returns></returns>
    template <typename OutputType, typename InputType>
    void InvokeTwoInputBlueprintFunction(TObjectPtr<UFunction> Function,
                                         OutputType* OutputValue,
                                         InputType* InInputValue);

    template <typename InputType>
    void InvokeTwoInputBlueprintFunction(TObjectPtr<UFunction> Function,
                                         InputType* InInputValue1,
                                         InputType* InInputValue2);

    /// <summary>
    /// 블루프린트의 메서드를 안정적으로 호출하기 위해 템플릿으로 작성된 메서드
    /// <br/>
    /// StaticClass()나 GetClass()의 FindFunctionByName를 이용해서 찾은 메서드를 실행할 수 있습니다.
    /// </summary>
    /// <param name="Function">이 인스턴스에서 실행할 수 있는 블루프린트 함수</param>
    /// <param name="InInputValue">입력으로 사용할 값</param>
    /// <returns></returns>
    template <typename InputType>
    void InvokeInputOnlyBlueprintFunction(TObjectPtr<UFunction> Function,
                                          InputType* InInputValue);

    /// <summary>
    /// 블루프린트의 메서드를 안정적으로 호출하기 위해 템플릿으로 작성된 메서드
    /// <br/>
    /// StaticClass()나 GetClass()의 FindFunctionByName를 이용해서 찾은 메서드를 실행할 수 있습니다.
    /// </summary>
    /// <param name="Function">이 인스턴스에서 실행할 수 있는 블루프린트 함수</param>
    /// <param name="OutputValue"></param>
    /// <returns></returns>
    template <typename OutputType>
    void InvokeOutputOnlyBlueprintFunction(TObjectPtr<UFunction> Function,
                                           OutputType* OutputValue);
};

template <typename OutputType, typename InputType>
void UDialogueGraphDirector::InvokeTwoInputBlueprintFunction(TObjectPtr<UFunction> Function,
                                                             OutputType* OutputValue,
                                                             InputType* InInputValue)
{
    // Function이 현재 클래스 인스턴스에서 실행할 수 있는지 확인
    ensure(Function->GetOwnerClass()->IsChildOf(GetClass()));

    // 함수를 실행하는 데 필요한 공간을 마련
    const TSharedRef<FStructOnScope> FuncParams = MakeShared<FStructOnScope>(Function);

    // 사용되는 프로퍼티를 정의
    const FProperty* InputProperty  = nullptr;
    const FProperty* OutputProperty = nullptr;

    // 프로퍼티를 유형별로 처리
    for ( TFieldIterator<FProperty> It(Function); It; ++It )
    {
        // 반환 유형인 프로퍼티 ( CPF_OutParam(보통 이쪽임), CPF_ReturnParam )
        if ( It->HasAnyPropertyFlags(CPF_ReturnParm | CPF_OutParm) )
        {
            OutputProperty = *It;
            continue;
        }
        // 입력에 대한 처리
        if ( It->HasAnyPropertyFlags(CPF_Parm) && !It->HasAnyPropertyFlags(CPF_ReturnParm | CPF_OutParm) )
        {
            InputProperty = *It;
        }
    }

    check(InputProperty && OutputProperty)

    // 프로퍼티를 이용해서 파라메터를 초기화
    if ( CastField<FObjectProperty>(InputProperty) )
    {
        InputProperty->SetValue_InContainer(FuncParams->GetStructMemory(), &InInputValue);
    }
    else
    {
        InputProperty->SetValue_InContainer(FuncParams->GetStructMemory(), InInputValue);
    }

    // 블루프린트 메서드를 호출
    this->ProcessEvent(Function, FuncParams->GetStructMemory());

    // 프로퍼티를 이용해서 결과를 추출
    if ( CastField<FObjectProperty>(OutputProperty) )
    {
        OutputProperty->GetValue_InContainer(FuncParams->GetStructMemory(), &OutputValue);
    }
    else
    {
        OutputProperty->GetValue_InContainer(FuncParams->GetStructMemory(), OutputValue);
    }
}

template <typename InputType>
void UDialogueGraphDirector::InvokeTwoInputBlueprintFunction(TObjectPtr<UFunction> Function,
                                                             InputType* InInputValue1,
                                                             InputType* InInputValue2)
{
    // Function이 현재 클래스 인스턴스에서 실행할 수 있는지 확인
    ensure(Function->GetOwnerClass()->IsChildOf(GetClass()));

    // 함수를 실행하는 데 필요한 공간을 마련
    const TSharedRef<FStructOnScope> FuncParams = MakeShared<FStructOnScope>(Function);

    // 사용되는 프로퍼티를 정의
    TFieldIterator<FProperty> It(Function);
    const FProperty* InputProperty1 = *It;
    ++It;
    const FProperty* InputProperty2 = *(It);


    check(InputProperty1)

    // 프로퍼티를 이용해서 파라메터를 초기화
    InputProperty1->SetValue_InContainer(FuncParams->GetStructMemory(), &InInputValue1);
    InputProperty2->SetValue_InContainer(FuncParams->GetStructMemory(), &InInputValue2);

    // 블루프린트 메서드를 호출
    this->ProcessEvent(Function, FuncParams->GetStructMemory());
}

template <typename InputType>
void UDialogueGraphDirector::InvokeInputOnlyBlueprintFunction(TObjectPtr<UFunction> Function,
                                                              InputType* InInputValue)
{
    // Function이 현재 클래스 인스턴스에서 실행할 수 있는지 확인
    ensure(Function->GetOwnerClass()->IsChildOf(GetClass()));

    // 함수를 실행하는 데 필요한 공간을 마련
    const TSharedRef<FStructOnScope> FuncParams = MakeShared<FStructOnScope>(Function);

    // 사용되는 프로퍼티를 정의
    const FProperty* InputProperty = nullptr;

    // 프로퍼티를 유형별로 처리
    for ( TFieldIterator<FProperty> It(Function); It; ++It )
    {
        // 입력에 대한 처리
        if ( It->HasAnyPropertyFlags(CPF_Parm) && !It->HasAnyPropertyFlags(CPF_ReturnParm | CPF_OutParm) )
        {
            InputProperty = *It;
        }
    }

    check(InputProperty)

    // 프로퍼티를 이용해서 파라메터를 초기화
    if ( CastField<FObjectProperty>(InputProperty) )
    {
        InputProperty->SetValue_InContainer(FuncParams->GetStructMemory(), &InInputValue);
    }
    else
    {
        InputProperty->SetValue_InContainer(FuncParams->GetStructMemory(), InInputValue);
    }

    // 블루프린트 메서드를 호출
    this->ProcessEvent(Function, FuncParams->GetStructMemory());
}

template <typename OutputType>
void UDialogueGraphDirector::InvokeOutputOnlyBlueprintFunction(TObjectPtr<UFunction> Function,
                                                               OutputType* OutputValue)
{
    // Function이 현재 클래스 인스턴스에서 실행할 수 있는지 확인
    ensure(Function->GetOwnerClass()->IsChildOf(GetClass()));

    // 함수를 실행하는 데 필요한 공간을 마련
    const TSharedRef<FStructOnScope> FuncParams = MakeShared<FStructOnScope>(Function);

    // 사용되는 프로퍼티를 정의
    const FProperty* OutputProperty = nullptr;

    // 프로퍼티를 유형별로 처리
    for ( TFieldIterator<FProperty> It(Function); It; ++It )
    {
        // 반환 유형인 프로퍼티 ( CPF_OutParam(보통 이쪽임), CPF_ReturnParam )
        if ( It->HasAnyPropertyFlags(CPF_ReturnParm | CPF_OutParm) )
        {
            OutputProperty = *It;
        }
    }

    check(OutputProperty)

    // 블루프린트 메서드를 호출
    this->ProcessEvent(Function, FuncParams->GetStructMemory());

    // 프로퍼티를 이용해서 결과를 추출
    if ( CastField<FObjectProperty>(OutputProperty) )
    {
        OutputProperty->GetValue_InContainer(FuncParams->GetStructMemory(), &OutputValue);
    }
    else
    {
        OutputProperty->GetValue_InContainer(FuncParams->GetStructMemory(), OutputValue);
    }
}
