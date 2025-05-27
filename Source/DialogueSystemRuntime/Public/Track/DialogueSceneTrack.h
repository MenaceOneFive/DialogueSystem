// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovieSceneTrack.h"
#include "DialogueSceneTrack.generated.h"

class UPlayerControlSection;
/**
 * 
 */
UCLASS(Abstract)
class DIALOGUESYSTEMRUNTIME_API UDialogueSceneTrack : public UMovieSceneTrack
{
	GENERATED_BODY()

public:
	UDialogueSceneTrack(const FObjectInitializer& Initializer): Super(Initializer)
	{
	};

	void Initialize(const FName InTrackName,
	                const bool bInSupportsMultipleRows,
	                const TSubclassOf<UMovieSceneSection>& InSupportedSectionType)
	{
		TrackName             = InTrackName;
		bSupportsMultipleRows = bInSupportsMultipleRows;
		SupportedSectionType  = InSupportedSectionType;
	}

public:
	virtual FName GetTrackName() const override;
	void SetTrackName(const FName& InTrackName);
	void SetSupportsMultipleRows(bool bInSupportsMultipleRows);
	virtual bool SupportsMultipleRows() const override;

	virtual FString GetDesc() override;
	virtual FText GetDisplayName() const override;
	void SetDisplayName(const FText& InDisplayName);
	void SetDescription(const FString& InDescription);

	virtual UMovieSceneSection* CreateNewSection() override;
	virtual const TArray<UMovieSceneSection*>& GetAllSections() const override;

	virtual bool IsEmpty() const override;
	virtual void AddSection(UMovieSceneSection& InSection) override;
	virtual bool HasSection(const UMovieSceneSection& Section) const override;
	virtual void RemoveSection(UMovieSceneSection& Section) override;
	virtual void RemoveSectionAt(int32 SectionIndex) override;

private:
	UPROPERTY()
	FName TrackName;

	UPROPERTY()
	FText DisplayName;

	UPROPERTY()
	bool bSupportsMultipleRows = false;

	UPROPERTY()
	TArray<UMovieSceneSection*> Sections;

	UPROPERTY()
	TSubclassOf<UMovieSceneSection> SupportedSectionType;

	UPROPERTY()
	FString Description;
};
