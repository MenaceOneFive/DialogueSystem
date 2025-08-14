// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueSystemRuntimeModule.h"

void FDialogueSystemRuntimeModule::ShutdownModule()
{
    IModuleInterface::ShutdownModule();
}

void FDialogueSystemRuntimeModule::StartupModule()
{
    IModuleInterface::StartupModule();
}

IMPLEMENT_MODULE(FDialogueSystemRuntimeModule, DialogueSystemRuntime)
