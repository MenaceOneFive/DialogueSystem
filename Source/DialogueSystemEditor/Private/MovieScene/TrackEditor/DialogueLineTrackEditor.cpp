// Fill out your copyright notice in the Description page of Project Settings.

#include "MovieScene/TrackEditor/DialogueLineTrackEditor.h"

#include "AnimatedRange.h"
#include "ISequencerModule.h"
#include "LevelSequence.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Character/DialogueCharacterAsset.h"
#include "Editor/Sequencer/Private/Sequencer.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Graph/DialogueEdGraph.h"
#include "Graph/DialogueGraph.h"
#include "Graph/Node/DialogueEdGraphDialogueLineNode.h"
#include "Graph/Node/DialogueEdGraphNode.h"
#include "MVVM/Views/ViewUtilities.h"
#include "Section/DialogueLineSection.h"
#include "Track/DialogueLineTrack.h"

#define LOCTEXT_NAMESPACE "FDialogueLineTrackEditor"

// ReSharper disable once CppPassValueParameterByConstReference
FDialogueLineTrackEditor::FDialogueLineTrackEditor(TSharedRef<ISequencer> InSequencer)
    : FMovieSceneTrackEditor(InSequencer)
{
    // 생성자 구현
}

FDialogueLineTrackEditor::~FDialogueLineTrackEditor()
{
    // 소멸자 구현
}

// ReSharper disable once CppPassValueParameterByConstReference
TSharedRef<ISequencerTrackEditor> FDialogueLineTrackEditor::CreateTrackEditor(TSharedRef<ISequencer> Sequencer)
{
    return MakeShareable(new FDialogueLineTrackEditor(Sequencer));
}

bool FDialogueLineTrackEditor::SupportsType(const TSubclassOf<UMovieSceneTrack> TrackClass) const
{
    return TrackClass == UDialogueLineTrack::StaticClass();
}

UMovieSceneTrack* FDialogueLineTrackEditor::AddTrack(UMovieScene* FocusedMovieScene,
                                                     const FGuid& ObjectHandle,
                                                     TSubclassOf<UMovieSceneTrack> TrackClass,
                                                     FName UniqueTypeName)
{
    return FocusedMovieScene->AddTrack(TrackClass, ObjectHandle);
}

void FDialogueLineTrackEditor::BuildAddTrackMenu(FMenuBuilder& MenuBuilder)
{
    // 트랙 이름 및 설명 텍스트 생성
    FText DisplayName = FText::FromString("DialogueLineTrack");
    FText InLabel     = FText::Format(NSLOCTEXT("Sequencer", "AddDialogueLineTrack", "{0}"), DisplayName);
    FText InToolTip   = FText::Format(NSLOCTEXT("Sequencer", "AddDialogueLineTrackTooltip", "대사 라인 트랙을 추가합니다: {0}"), DisplayName);

    // 트랙 추가 가능 여부 람다 정의
    FCanExecuteAction CanAddSection = FCanExecuteAction::CreateLambda([this]
    {
        if ( GetSequencer().IsValid() )
        {
            ISequencer* Sequencer = GetSequencer().Get();
            return !Sequencer->IsReadOnly();
        }
        return false;
    });

    // 트랙 추가 액션 람다 정의
    FExecuteAction AddSection = FExecuteAction::CreateLambda([this]
    {
        UMovieScene* FocusedSequence = GetFocusedMovieScene();
        if ( FocusedSequence && !FocusedSequence->IsReadOnly() )
        {
            // DialogueLineTrack을 추가
            FocusedSequence->AddTrack(UDialogueLineTrack::StaticClass());
        }
    });

    // 메뉴에 엔트리 추가
    MenuBuilder.AddMenuEntry(InLabel, InToolTip, FSlateIcon(), FUIAction(AddSection, CanAddSection));
}

TSharedPtr<SWidget> FDialogueLineTrackEditor::BuildOutlinerEditWidget(const FGuid& ObjectBinding,
                                                                      UMovieSceneTrack* Track,
                                                                      const FBuildEditWidgetParams& Params)
{
    TWeakPtr<ISequencer> WeakSequencer = GetSequencer();

    const int32 RowIndex = Params.TrackInsertRowIndex;
    auto SubMenuCallback = [this, RowIndex,Track, WeakSequencer]() -> TSharedRef<SWidget>
    {
        FMenuBuilder MenuBuilder(true, nullptr);

        PopulateMenu_AddSection(MenuBuilder, "DialogueTrackSection", RowIndex, Track, WeakSequencer);

        return MenuBuilder.MakeWidget();
    };

    return MakeAddButton(LOCTEXT("AddSection", "Section"), FOnGetContent::CreateLambda(SubMenuCallback), Params.ViewModel);
}

void FDialogueLineTrackEditor::PopulateMenu_AddSection(FMenuBuilder& MenuBuilder,
                                                       const FName DisplayName,
                                                       int32 RowIndex,
                                                       UMovieSceneTrack* Track,
                                                       TWeakPtr<ISequencer> InSequencer)
{
    if ( !Track )
    {
        return;
    }

    const FExecuteAction AddSection = FExecuteAction::CreateLambda([Track, InSequencer, RowIndex]
    {
        const TSharedPtr<ISequencer> Sequencer = InSequencer.Pin();
        if ( !Sequencer.IsValid() )
        {
            return;
        }

        const FQualifiedFrameTime CurrentTime = Sequencer->GetLocalTime();
        const FFrameNumber PlaybackEnd        = UE::MovieScene::DiscreteExclusiveUpper(Sequencer->GetFocusedMovieSceneSequence()->GetMovieScene()->GetPlaybackRange());

        int32 SpecifiedRowIndex = RowIndex;

        FScopedTransaction Transaction(LOCTEXT("AddSectionTransactionText", "Add Section"));
        if ( UMovieSceneSection* NewSection = Track->CreateNewSection() )
        {
            int32 OverlapPriority = 0;
            TMap<int32, int32> NewToOldRowIndices;
            //if creating with an override force the row index to be last
            if ( Track->GetSupportedBlendTypes().Contains(EMovieSceneBlendType::Override) )
            {
                SpecifiedRowIndex = Track->GetMaxRowIndex() + 1;
            }
            for ( UMovieSceneSection* Section : Track->GetAllSections() )
            {
                OverlapPriority = FMath::Max(Section->GetOverlapPriority() + 1, OverlapPriority);

                // Move existing sections on the same row or beyond so that they don't overlap with the new section
                if ( Section != NewSection && Section->GetRowIndex() >= SpecifiedRowIndex )
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
            NewSection->SetRowIndex(SpecifiedRowIndex);

            Track->AddSection(*NewSection);
            Track->UpdateEasing();

            if ( UMovieSceneNameableTrack* NameableTrack = Cast<UMovieSceneNameableTrack>(Track) )
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
    const FCanExecuteAction CanAddSection = FCanExecuteAction::CreateLambda([InSequencer] { return InSequencer.IsValid() && !InSequencer.Pin()->IsReadOnly(); });

    const FText InLabel   = FText::Format(LOCTEXT("AddSection", "{0}"), FText::FromName(DisplayName));
    const FText InToolTip = FText::Format(LOCTEXT("AddSectionFormatToolTip", "Adds a new {0}"), FText::FromName(DisplayName));

    MenuBuilder.AddMenuEntry(InLabel,
                             InToolTip,
                             FSlateIcon(),
                             FUIAction(AddSection, CanAddSection));
}

TSharedRef<ISequencerSection> FDialogueLineTrackEditor::MakeSectionInterface(UMovieSceneSection& SectionObject,
                                                                             UMovieSceneTrack& Track,
                                                                             FGuid ObjectBinding)
{
    return MakeShared<FDialogueLineSequencerSection>(SectionObject, AsWeak());
}

void FDialogueLineSequencerSection::BuildSectionContextMenu(FMenuBuilder& MenuBuilder,
                                                            const FGuid& ObjectBinding)
{
    FSequencerSection::BuildSectionContextMenu(MenuBuilder, ObjectBinding);
    if ( auto Section = this->WeakSection.Pin() )
    {
        if ( auto Editor = WeakEditor.Pin() )
        {
            MenuBuilder.BeginSection(FName("DialogueLineSection"));
            MenuBuilder.AddSubMenu(FText::FromString("DialogueLine"), FText::FromString(""),
                                   FNewMenuDelegate::CreateLambda([Section, this](FMenuBuilder& SubMenu)
                                   {
                                       SubMenu.AddMenuEntry(FText::FromString("Import Dialogue Line"),
                                                            FText::FromString("Import from referencing graph"),
                                                            FSlateIcon(),
                                                            FUIAction(FExecuteAction::CreateSP(this, &FDialogueLineSequencerSection::OnImportDialogueLineClicked))
                                                           );
                                   }));
            MenuBuilder.EndSection();
        }
    }
}

/// <summary>
/// "Import Dialogue Line" 메뉴 클릭 시 동작.<br>
/// Section이 유효하면, LevelSequence의 아우터를 대상으로 UPROPERTY 참조 로그를 출력한다.
/// <param name="Section">대상 시퀀서 섹션의 WeakPtr</param>
/// </summary>
void FDialogueLineSequencerSection::OnImportDialogueLineClicked()
{
    if ( const auto Section = WeakSection.Pin() )
    {
        if ( Section.IsValid() )
        {
            if ( const auto LevelSequence = Section->GetTypedOuter<ULevelSequence>() )
            {
                if ( auto Result = this->LogAllUPROPERTYReferencers(LevelSequence->GetOuter());
                    !Result.IsEmpty() )
                {
                    if ( const auto LineSection = Cast<UDialogueLineSection>(Section.Get()) )
                    {
                        LineSection->CharacterName = Result[0].CharacterName;
                        LineSection->DialogueLine  = Result[0].DialogueLine;
                    }
                }
            }
        }
    }
}

TArray<FReferencingDialogueLineSearchResult> FDialogueLineSequencerSection::LogAllUPROPERTYReferencers(UObject* InTargetAsset)
{
    if ( !InTargetAsset )
    {
        UE_LOG(LogTemp, Warning, TEXT("Target Asset is null."));
        return {};
    }

    UE_LOG(LogTemp, Log, TEXT("Searching for UPROPERTY references to asset: %s"), *InTargetAsset->GetPathName());

    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    IAssetRegistry& AssetRegistry             = AssetRegistryModule.Get();

    TArray<FName> ReferencingAssetNames;
    AssetRegistry.GetReferencers(FName(*InTargetAsset->GetPathName()), ReferencingAssetNames);

    UE_LOG(LogTemp, Log, TEXT("Found %d assets referencing %s (AssetRegistry 기준)"), ReferencingAssetNames.Num(), *InTargetAsset->GetPathName());

    const auto LevelSequence = StaticLoadObject(UObject::StaticClass(), nullptr, *InTargetAsset->GetPathName());

    check(LevelSequence)

    TArray<FReferencingDialogueLineSearchResult> Results;

    for ( const FName& ReferencerName : ReferencingAssetNames )
    {
        UObject* ReferencerObject = StaticLoadObject(UObject::StaticClass(), nullptr, *ReferencerName.ToString());

        if ( const auto GraphAsset = Cast<UDialogueGraph>(ReferencerObject) )
        {
            const auto Graph = Cast<UDialogueEdGraphAssetData>(GraphAsset->AssetUserData);
            check(Graph)
            for ( const auto Nodes = Graph->DialogueEdGraph->GetNodes();
                  const UDialogueEdGraphNode* Node : Nodes )
            {
                const auto LineNode = Cast<UDialogueEdGraphDialogueLineNode>(Node);
                if ( !LineNode )
                {
                    continue;
                }
                if ( LineNode->LevelSequenceToPlay == LevelSequence )
                {
                    const auto Name         = LineNode->DialogueCharacterAsset ? LineNode->DialogueCharacterAsset->Name : "";
                    const auto DialogueLine = LineNode->DialogueLine;
                    Results.Add({Name, DialogueLine});
                    UE_LOG(LogTemp, Log, TEXT("찾음"));
                }
            }
        }
    }
    return Results;
}

#undef LOCTEXT_NAMESPACE
