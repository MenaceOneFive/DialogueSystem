// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;


public class DialogueSystemEditor : ModuleRules
{
    public DialogueSystemEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDependencyModuleNames.AddRange(new string[] { "EditorStyle" });
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            [
            ]
        );

        PrivateIncludePaths.AddRange(
            [
                // ... add other private include paths required here ...
            ]
        );

        PublicDependencyModuleNames.AddRange(
            [
                "Engine",
                "Core",
                "CoreUObject",
                "Slate",
                "SlateCore",
                "InputCore",
                "UnrealEd",
                "MovieScene",
                "MovieSceneTracks",
                "Sequencer",
                "SequencerCore",
                "DialogueSystemRuntime",
                "GraphEditor",
                "ToolMenus",
                "GenAPIBase",
                "GeminiAPIWrapper",
                "OpenAI_APIWrapper",
                "Json",
                "JsonUtilities",
                "Projects",
                "EditorStyle",
                "EditorWidgets",
                "PropertyEditor",
                "LevelSequence",
                "UMG",
                "BlueprintGraph",
                "ApplicationCore",
                "GameplayAbilities",
                "GameplayTags",
                "GameplayTasks"
            ]
        );

        PrivateDependencyModuleNames.AddRange(
            [
                "BehaviorTreeEditor", "NiagaraEditor"
            ]
        );

        DynamicallyLoadedModuleNames.AddRange(
            []
        );
    }
}