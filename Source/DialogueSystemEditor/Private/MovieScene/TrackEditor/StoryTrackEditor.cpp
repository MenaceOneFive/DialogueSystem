// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueSystemEditor/Public/MovieScene/TrackEditor/StoryTrackEditor.h"

#include "Track/StoryEventTrack.h"

FStoryTrackEditor::FStoryTrackEditor(const TSharedRef<ISequencer>& InSequencer): FMovieSceneTrackEditor(InSequencer)
{
}

FStoryTrackEditor::~FStoryTrackEditor()
{
}

TSharedRef<ISequencerTrackEditor> FStoryTrackEditor::CreateTrackEditor(TSharedRef<ISequencer> Sequencer)
{
    return MakeShared<FStoryTrackEditor>(Sequencer);
}

bool FStoryTrackEditor::SupportsType(TSubclassOf<class UMovieSceneTrack> TrackClass) const
{
    return TrackClass->IsChildOf(UStoryEventTrack::StaticClass());
}
