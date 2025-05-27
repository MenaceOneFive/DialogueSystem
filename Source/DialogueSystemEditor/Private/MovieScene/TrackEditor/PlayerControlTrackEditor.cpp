// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueSystemEditor/Public/MovieScene/TrackEditor/PlayerControlTrackEditor.h"
#include "Section/PlayerControlSection.h"

FPlayerControlTrackEditor::FPlayerControlTrackEditor(const TSharedRef<ISequencer>& InSequencer): FMovieSceneTrackEditor(InSequencer)
{
}

FPlayerControlTrackEditor::~FPlayerControlTrackEditor()
{
}

TSharedRef<ISequencerTrackEditor> FPlayerControlTrackEditor::CreateTrackEditor(TSharedRef<ISequencer> Sequencer)
{
    return MakeShared<FPlayerControlTrackEditor>(Sequencer);
}

bool FPlayerControlTrackEditor::SupportsType(TSubclassOf<class UMovieSceneTrack> TrackClass) const
{
    return TrackClass->IsChildOf(UPlayerControlSection::StaticClass());
}
