// Fill out your copyright notice in the Description page of Project Settings.
#include "Section/StoryEventSection.h"

UStoryEventSection::UStoryEventSection(): UMovieSceneSection()
{
	SetColorTint(FColor::Green);
	bSupportsInfiniteRange = false;
}

