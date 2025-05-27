// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovieSceneTrackEditor.h"

/**
 * 
 */
class DIALOGUESYSTEMEDITOR_API FDialogueTrackEditor final : public FMovieSceneTrackEditor
{
public:
	FDialogueTrackEditor(const TSharedRef<ISequencer>& InSequencer);
	virtual ~FDialogueTrackEditor() override;
	static TSharedRef<ISequencerTrackEditor> CreateTrackEditor(TSharedRef<ISequencer> Sequencer);

public:
	virtual bool SupportsType(TSubclassOf<class UMovieSceneTrack> TrackClass) const override;
	virtual UMovieSceneTrack* AddTrack(UMovieScene* FocusedMovieScene, const FGuid& ObjectHandle, TSubclassOf<class UMovieSceneTrack> TrackClass, FName UniqueTypeName) override;
	virtual void BuildAddTrackMenu(FMenuBuilder& MenuBuilder) override;
	virtual TSharedPtr<SWidget> BuildOutlinerEditWidget(const FGuid& ObjectBinding, UMovieSceneTrack* Track, const FBuildEditWidgetParams& Params) override;
};
