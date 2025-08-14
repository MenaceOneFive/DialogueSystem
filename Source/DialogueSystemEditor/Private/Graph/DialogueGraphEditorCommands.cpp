// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/DialogueGraphEditorCommands.h"

#define LOCTEXT_NAMESPACE "DialogueGraphEditor"

void FDialogueGraphEditorCommands::RegisterCommands()
{
    UI_COMMAND(Command1, "Command1", "I don't know what to do", EUserInterfaceActionType::Button, FInputChord());
    // C키를 누르면 선택한 영역에 대해 주석 박스를 생성할 수 있다.
    UI_COMMAND(CommentSelection, "CommentSelection", "Comment Selected nodes", EUserInterfaceActionType::Button, FInputChord(EKeys::C));
    UI_COMMAND(DeleteSelectedNode, "DeleteSelectedNode", "Delete Selected Node", EUserInterfaceActionType::Button, FInputChord());
    UI_COMMAND(DeleteAllNodeConnection, "DeleteAllNodeConnection", "Delete All Node Connection", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
