#pragma once
#include "AnimatedRange.h"
#include "ISequencer.h"
#include "MovieScene.h"
#include "MovieSceneNameableTrack.h"
#include "MovieSceneTimeHelpers.h"
#include "MovieSceneTrack.h"

#define LOCTEXT_NAMESPACE "SequencerUtil"

namespace Vsk::MovieScene::Util
{
    // ReSharper disable once CppMemberFunctionMayBeStatic
    inline void PopulateMenu_CreateNewSection(FMenuBuilder& MenuBuilder,
                                              int32 RowIndex,
                                              UMovieSceneTrack* Track,
                                              TWeakPtr<ISequencer> InSequencer,
                                              TArray<TSubclassOf<UMovieSceneSection>> SupportedSectionType = TArray<TSubclassOf<UMovieSceneSection>>())
    {
        if ( !Track )
        {
            return;
        }

        auto CreateNewSection = [=](const TSubclassOf<UMovieSceneSection>& SectionClass)
        {
            TSharedPtr<ISequencer> Sequencer = InSequencer.Pin();
            if ( !Sequencer.IsValid() )
            {
                return;
            }

            checkf(Track->SupportsType(SectionClass), TEXT("현재 Track에서 허용되지 않는 Section 유형. Track 클래스나 TrackEditor 클래스를 확인"));

            FQualifiedFrameTime CurrentTime = Sequencer->GetLocalTime();
            FFrameNumber PlaybackEnd        = UE::MovieScene::DiscreteExclusiveUpper(Sequencer->GetFocusedMovieSceneSequence()->GetMovieScene()->GetPlaybackRange());

            FScopedTransaction Transaction(LOCTEXT("AddSectionTransactionText", "Add Section"));
            if ( UMovieSceneSection* NewSection = NewObject<UMovieSceneSection>(Track, SectionClass) )
            {
                int32 OverlapPriority = 0;
                TMap<int32, int32> NewToOldRowIndices;
                //if creating with an override force the row index to be last
                // if (Track->GetSupportedBlendTypes().Contains(EMovieSceneBlendType::Override))
                // {
                // 	SpecifiedRowIndex = Track->GetMaxRowIndex() + 1;
                // }
                for ( UMovieSceneSection* Section : Track->GetAllSections() )
                {
                    OverlapPriority = FMath::Max(Section->GetOverlapPriority() + 1, OverlapPriority);

                    // Move existing sections on the same row or beyond so that they don't overlap with the new section
                    if ( Section != NewSection && Section->GetRowIndex() >= RowIndex )
                    {
                        int32 OldRowIndex = Section->GetRowIndex();
                        int32 NewRowIndex = Section->GetRowIndex() + 1;
                        NewToOldRowIndices.FindOrAdd(NewRowIndex, OldRowIndex);
                        Section->Modify();
                        Section->SetRowIndex(NewRowIndex);
                    }
                }

                Track->Modify();

                Track->OnRowIndicesChanged(NewToOldRowIndices);

                if ( Sequencer->GetInfiniteKeyAreas() && NewSection->GetSupportsInfiniteRange() )
                {
                    NewSection->SetRange(TRange<FFrameNumber>::All());
                }
                else
                {
                    FFrameNumber NewSectionRangeEnd = PlaybackEnd;
                    if ( PlaybackEnd <= CurrentTime.Time.FrameNumber )
                    {
                        const FAnimatedRange ViewRange  = Sequencer->GetViewRange();
                        const FFrameRate TickResolution = Sequencer->GetFocusedTickResolution();
                        NewSectionRangeEnd              = (ViewRange.GetUpperBoundValue() * TickResolution).FloorToFrame();
                    }

                    NewSection->SetRange(TRange<FFrameNumber>(CurrentTime.Time.FrameNumber, NewSectionRangeEnd));
                }

                NewSection->SetOverlapPriority(OverlapPriority);
                NewSection->SetRowIndex(RowIndex);

                Track->AddSection(*NewSection);
                Track->UpdateEasing();

                if ( UMovieSceneNameableTrack* NameableTrack = Cast<UMovieSceneNameableTrack>(Track) )
                {
                    NameableTrack->SetTrackRowDisplayName(FText::GetEmpty(), RowIndex);
                }

                Sequencer->NotifyMovieSceneDataChanged(EMovieSceneDataChangeType::MovieSceneStructureItemAdded);
            }
            else
            {
                Transaction.Cancel();
            }
        };

        for ( const TSubclassOf<UMovieSceneSection>& SectionType : SupportedSectionType )
        {
            FText NameOverride = FText::Format(LOCTEXT("AddSectionText", "{0}을 추가"),
                                               FText::FromString(SectionType->GetName()));
            FText TooltipOverride = FText::Format(LOCTEXT("AddSectionToolTip", "{0}을 추가"),
                                                  FText::FromString(SectionType->GetName()));

            MenuBuilder.AddMenuEntry(NameOverride,
                                     TooltipOverride,
                                     FSlateIcon(),
                                     FUIAction(FExecuteAction::CreateLambda(CreateNewSection, SectionType),
                                               FCanExecuteAction::CreateLambda([InSequencer] { return InSequencer.IsValid() && !InSequencer.Pin()->IsReadOnly(); })));
        }
    }
}

#undef LOCTEXT_NAMESPACE
