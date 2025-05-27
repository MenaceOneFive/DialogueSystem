// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovieSceneTrackEditor.h"

/**
 * 
 */
class DIALOGUESYSTEMEDITOR_API FStoryTrackEditor final : public FMovieSceneTrackEditor
{
public:
	FStoryTrackEditor(const TSharedRef<ISequencer>& InSequencer);
	virtual ~FStoryTrackEditor() override;
	static TSharedRef<ISequencerTrackEditor> CreateTrackEditor(TSharedRef<ISequencer> Sequencer);
	virtual bool SupportsType(TSubclassOf<class UMovieSceneTrack> TrackClass) const override;
};
