// Fill out your copyright notice in the Description page of Project Settings.


#include "Track/DialogueLineTrack.h"
#include "Section/DialogueLineSection.h"
#include "Track/DialogueLineTrackInstance.h"

#define LOCTEXT_NAMESPACE "DialogueLineTrack"

UDialogueLineTrack::UDialogueLineTrack()
{
    bSupportsMultipleRows = false;
    bSupportsBlending     = false;
    SupportedSections     = {UDialogueLineSection::StaticClass()};
    TrackInstanceType     = UDialogueLineTrackInstance::StaticClass();
}

FName UDialogueLineTrack::GetTrackName() const
{
    return FName("Dialogue Line Track");
}

#if WITH_EDITORONLY_DATA
FText UDialogueLineTrack::GetDisplayName() const
{
    return LOCTEXT("DialogueLineTrack", "Dialogue Line Track");
}
#endif

#undef LOCTEXT_NAMESPACE
