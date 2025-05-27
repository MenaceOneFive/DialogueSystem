// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueSystemEditor/Public/MovieScene/TrackEditor/DialogueTrackEditor.h"

#include "AnimatedRange.h"
#include "MovieSceneNameableTrack.h"
#include "MVVM/Views/ViewUtilities.h"
#include "Track/DialogueEventTrack.h"

#define LOCTEXT_NAMESPACE "FDialogueTrackEditor"

void PopulateMenu_AddSection(FMenuBuilder& MenuBuilder,
                             FName DisplayName,
                             int32 RowIndex,
                             UMovieSceneTrack* Track,
                             TWeakPtr<ISequencer> InSequencer);

FDialogueTrackEditor::FDialogueTrackEditor(const TSharedRef<ISequencer>& InSequencer): FMovieSceneTrackEditor(InSequencer)
{
}

FDialogueTrackEditor::~FDialogueTrackEditor()
{
}

TSharedRef<ISequencerTrackEditor> FDialogueTrackEditor::CreateTrackEditor(TSharedRef<ISequencer> Sequencer)
{
    return MakeShared<FDialogueTrackEditor>(Sequencer);
}

bool FDialogueTrackEditor::SupportsType(TSubclassOf<class UMovieSceneTrack> TrackClass) const
{
    return TrackClass->IsChildOf(UDialogueEventTrack::StaticClass());
}

UMovieSceneTrack* FDialogueTrackEditor::AddTrack(UMovieScene* FocusedMovieScene,
                                                 const FGuid& ObjectHandle,
                                                 TSubclassOf<class UMovieSceneTrack> TrackClass,
                                                 FName UniqueTypeName)
{
    return FMovieSceneTrackEditor::AddTrack(FocusedMovieScene, ObjectHandle, TrackClass, UniqueTypeName);
}

void FDialogueTrackEditor::BuildAddTrackMenu(FMenuBuilder& MenuBuilder)
{
    FText DisplayName = FText::FromString("DialogueTrack");
    FText InLabel     = FText::Format(LOCTEXT("AddTrack", "{0}"), DisplayName);
    FText InToolTip   = FText::Format(LOCTEXT("AddTrackFormatToolTip", "Adds a new {0}"), DisplayName);

    FCanExecuteAction CanAddSection
            = FCanExecuteAction::CreateLambda([this]
            {
                if (GetSequencer().IsValid())
                {
                    ISequencer* Sequencer = GetSequencer().Get();
                    return !Sequencer->IsReadOnly();
                }
                return false;
            });
    FExecuteAction AddSection
            = FExecuteAction::CreateLambda([this]
            {
                UMovieScene* FocusedSequence = GetFocusedMovieScene();
                if (FocusedSequence && !FocusedSequence->IsReadOnly())
                {
                    FocusedSequence->AddTrack(UDialogueEventTrack::StaticClass());
                }
            });

    MenuBuilder.AddMenuEntry(InLabel,
                             InToolTip,
                             FSlateIcon(),
                             FUIAction(AddSection,
                                       CanAddSection));
}

TSharedPtr<SWidget> FDialogueTrackEditor::BuildOutlinerEditWidget(const FGuid& ObjectBinding,
                                                                  UMovieSceneTrack* Track,
                                                                  const FBuildEditWidgetParams& Params)
{
    TWeakPtr<ISequencer> WeakSequencer = GetSequencer();

    const int32 RowIndex = Params.TrackInsertRowIndex;
    auto SubMenuCallback = [=]() -> TSharedRef<SWidget>
    {
        FMenuBuilder MenuBuilder(true, nullptr);

        PopulateMenu_AddSection(MenuBuilder, "DialogueTrackSection", RowIndex, Track, WeakSequencer);

        return MenuBuilder.MakeWidget();
    };

    return MakeAddButton(LOCTEXT("AddSection", "Section"), FOnGetContent::CreateLambda(SubMenuCallback), Params.ViewModel);
}

void PopulateMenu_AddSection(FMenuBuilder& MenuBuilder,
                             const FName DisplayName,
                             int32 RowIndex,
                             UMovieSceneTrack* Track,
                             TWeakPtr<ISequencer> InSequencer)
{
    if (!Track)
    {
        return;
    }

    FExecuteAction AddSection = FExecuteAction::CreateLambda([Track, InSequencer, RowIndex]
    {
        TSharedPtr<ISequencer> Sequencer = InSequencer.Pin();
        if (!Sequencer.IsValid())
        {
            return;
        }

        FQualifiedFrameTime CurrentTime = Sequencer->GetLocalTime();
        FFrameNumber PlaybackEnd        = UE::MovieScene::DiscreteExclusiveUpper(Sequencer->GetFocusedMovieSceneSequence()->GetMovieScene()->GetPlaybackRange());

        int32 SpecifiedRowIndex = RowIndex;

        FScopedTransaction Transaction(LOCTEXT("AddSectionTransactionText", "Add Section"));
        if (UMovieSceneSection* NewSection = Track->CreateNewSection())
        {
            int32 OverlapPriority = 0;
            TMap<int32, int32> NewToOldRowIndices;
            //if creating with an override force the row index to be last
            if (Track->GetSupportedBlendTypes().Contains(EMovieSceneBlendType::Override))
            {
                SpecifiedRowIndex = Track->GetMaxRowIndex() + 1;
            }
            for (UMovieSceneSection* Section : Track->GetAllSections())
            {
                OverlapPriority = FMath::Max(Section->GetOverlapPriority() + 1, OverlapPriority);

                // Move existing sections on the same row or beyond so that they don't overlap with the new section
                if (Section != NewSection && Section->GetRowIndex() >= SpecifiedRowIndex)
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

            if (Sequencer->GetInfiniteKeyAreas() && NewSection->GetSupportsInfiniteRange())
            {
                NewSection->SetRange(TRange<FFrameNumber>::All());
            }
            else
            {
                FFrameNumber NewSectionRangeEnd = PlaybackEnd;
                if (PlaybackEnd <= CurrentTime.Time.FrameNumber)
                {
                    const FAnimatedRange ViewRange  = Sequencer->GetViewRange();
                    const FFrameRate TickResolution = Sequencer->GetFocusedTickResolution();
                    NewSectionRangeEnd              = (ViewRange.GetUpperBoundValue() * TickResolution).FloorToFrame();
                }

                NewSection->SetRange(TRange<FFrameNumber>(CurrentTime.Time.FrameNumber, NewSectionRangeEnd));
            }

            NewSection->SetOverlapPriority(OverlapPriority);
            NewSection->SetRowIndex(SpecifiedRowIndex);

            Track->AddSection(*NewSection);
            Track->UpdateEasing();

            if (UMovieSceneNameableTrack* NameableTrack = Cast<UMovieSceneNameableTrack>(Track))
            {
                NameableTrack->SetTrackRowDisplayName(FText::GetEmpty(), SpecifiedRowIndex);
            }

            Sequencer->NotifyMovieSceneDataChanged(EMovieSceneDataChangeType::MovieSceneStructureItemAdded);
        }
        else
        {
            Transaction.Cancel();
        }
    });
    FCanExecuteAction CanAddSection = FCanExecuteAction::CreateLambda([InSequencer] { return InSequencer.IsValid() && !InSequencer.Pin()->IsReadOnly(); });

    FText InLabel   = FText::Format(LOCTEXT("AddSection", "{0}"), FText::FromName(DisplayName));
    FText InToolTip = FText::Format(LOCTEXT("AddSectionFormatToolTip", "Adds a new {0}"), FText::FromName(DisplayName));

    MenuBuilder.AddMenuEntry(InLabel,
                             InToolTip,
                             FSlateIcon(),
                             FUIAction(AddSection, CanAddSection));
}
#undef LOCTEXT_NAMESPACE
