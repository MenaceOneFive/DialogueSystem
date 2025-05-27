// Fill out your copyright notice in the Description page of Project Settings.


#include "Track/DialogueSceneTrack.h"

#include "Section/PlayerControlSection.h"

bool UDialogueSceneTrack::SupportsMultipleRows() const
{
	return bSupportsMultipleRows;
}

FString UDialogueSceneTrack::GetDesc()
{
	return Description;
}

FText UDialogueSceneTrack::GetDisplayName() const
{
	return DisplayName.IsEmpty() ? FText::FromName(TrackName) : DisplayName;
}

FName UDialogueSceneTrack::GetTrackName() const
{
	return TrackName;
}

void UDialogueSceneTrack::SetTrackName(const FName& InTrackName)
{
	this->TrackName = InTrackName;
}

void UDialogueSceneTrack::SetSupportsMultipleRows(bool bInSupportsMultipleRows)
{
	this->bSupportsMultipleRows = bInSupportsMultipleRows;
}


void UDialogueSceneTrack::SetDisplayName(const FText& InDisplayName)
{
	this->DisplayName = InDisplayName;
}

void UDialogueSceneTrack::SetDescription(const FString& InDescription)
{
	this->Description = InDescription;
}

UMovieSceneSection* UDialogueSceneTrack::CreateNewSection()
{
	return NewObject<UMovieSceneSection>(this, SupportedSectionType.Get());
}

const TArray<UMovieSceneSection*>& UDialogueSceneTrack::GetAllSections() const
{
	return Sections;
}

bool UDialogueSceneTrack::IsEmpty() const
{
	return Sections.IsEmpty();
}

void UDialogueSceneTrack::AddSection(UMovieSceneSection& InSection)
{
	UClass* SectionClass = InSection.GetClass();
	if (SectionClass->IsChildOf(SupportedSectionType.Get()))
	{
		Sections.Add(&InSection);
	}
}

bool UDialogueSceneTrack::HasSection(const UMovieSceneSection& Section) const
{
	return Sections.Contains(&Section);
}

void UDialogueSceneTrack::RemoveSection(UMovieSceneSection& Section)
{
	if (SupportedSectionType)
	{
		Sections.Remove(&Section);
	}
}

void UDialogueSceneTrack::RemoveSectionAt(int32 SectionIndex)
{
	return Sections.RemoveAt(SectionIndex);
}
