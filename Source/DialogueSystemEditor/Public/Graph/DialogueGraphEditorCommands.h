// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

class DIALOGUESYSTEMEDITOR_API FDialogueGraphEditorCommands final : public TCommands<FDialogueGraphEditorCommands>
{
public:
    FDialogueGraphEditorCommands()
        : TCommands
        (
         TEXT("FDialogueGraphEditor"),                                                         // Context name for fast lookup
         NSLOCTEXT("Contexts", "FDialogueGraphEditor", "DialogueGraph Editor's command list"), // Localized context name for displaying
         NAME_None,                                                                            // Parent context name. 
         FAppStyle::GetAppStyleSetName()                                                       // Icon Style Set
        )
    {
    }

    TSharedPtr<FUICommandInfo> Command1;
    TSharedPtr<FUICommandInfo> DeleteSelectedNode;
    TSharedPtr<FUICommandInfo> DeleteAllNodeConnection;

    virtual void RegisterCommands() override;
};
