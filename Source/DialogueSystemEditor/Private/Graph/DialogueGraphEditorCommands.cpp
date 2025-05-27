// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/DialogueGraphEditorCommands.h"

#define LOCTEXT_NAMESPACE "DialogueGraphEditor"

void FDialogueGraphEditorCommands::RegisterCommands()
{
    UI_COMMAND(Command1, "Command1", "I don't know what to do", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
