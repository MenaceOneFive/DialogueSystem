// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueGraphBlueprintExtension.h"

#include "KismetCompiler.h"

void UDialogueGraphBlueprintExtension::HandlePreloadObjectsForCompilation(UBlueprint* OwningBlueprint)
{
    UE_LOG(LogTemp, Log, TEXT(""))
}

void UDialogueGraphBlueprintExtension::HandleGenerateFunctionGraphs(FKismetCompilerContext* CompilerContext)
{
    CompilerContext->OnFunctionListCompiled().AddLambda([](FKismetCompilerContext* Context)
    {
        UBlueprint* Blueprint = Context->Blueprint;
        if ( !Blueprint->GeneratedClass )
        {
            return;
        }
    });
    //     auto GeneratedClass = Blueprint->GeneratedClass;
    //     TArray<FName> FunctionList;
    //     GeneratedClass->GenerateFunctionList(FunctionList);
    //     TObjectPtr<UFunction> NewFunction = GeneratedClass->FindFunctionByName(FName("NewFunction"));
    //     TObjectPtr<UFunction> CustomEvent = GeneratedClass->FindFunctionByName(FName("CustomEvent"));
    //
    //     FField* Prop = NewFunction->ChildProperties;
    //     while ( Prop )
    //     {
    //         Prop = Prop->Next;
    //         UE_LOG(LogTemp, Log, TEXT(""))
    //     };
    //     Prop = CustomEvent->ChildProperties;
    //     while ( Prop )
    //     {
    //         Prop = Prop->Next;
    //         UE_LOG(LogTemp, Log, TEXT(""))
    //     };
    // });
    // UE_LOG(LogTemp, Log, TEXT(""))
}
