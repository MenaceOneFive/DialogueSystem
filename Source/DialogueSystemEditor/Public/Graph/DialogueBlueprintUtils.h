#pragma once

enum class EFunctionCreationResult
{
    OK,
    Failed_CorruptedBlueprint,
    Failed_DuplicatedName //해당 노드에 대한 메서드가 이미 존재합니다.
};

enum class EFunctionType
{
    CanSelectThisNode,
    WhenSelectThisNode
};

class FDialogueBlueprintUtils
{
public:
    FDialogueBlueprintUtils() = delete;
    static EFunctionCreationResult CreateWhenSelectedFunction(UBlueprint* Blueprint,
                                                              FName FunctionName);
    static EFunctionCreationResult CreateCanSelectThisNodeFunction(UBlueprint* Blueprint,
                                                                   FName FunctionName);
    static UEdGraph* FindFunctionGraph(UBlueprint* Blueprint,
                                       FName FunctionGraphName);
    static EFunctionCreationResult CreateFunctionGraph(UBlueprint* Blueprint,
                                                       EFunctionType FunctionType,
                                                       FName FunctionGraphName);
};
