// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovieSceneTrackEditor.h"

/**
 * 
 */
class DIALOGUESYSTEMEDITOR_API FPlayerControlTrackEditor final : public FMovieSceneTrackEditor
{
public:
	FPlayerControlTrackEditor(const TSharedRef<ISequencer>& InSequencer);
	virtual ~FPlayerControlTrackEditor() override;
	static TSharedRef<ISequencerTrackEditor> CreateTrackEditor(TSharedRef<ISequencer> Sequencer);
	virtual bool SupportsType(TSubclassOf<class UMovieSceneTrack> TrackClass) const override;
};
