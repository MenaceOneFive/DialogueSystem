// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovieSceneTrackEditor.h"

///<summary>
/// DialogueLineTrack 전용 트랙 에디터 클래스
///</summary>
class DIALOGUESYSTEMEDITOR_API FDialogueLineTrackEditor final : public FMovieSceneTrackEditor
{
public: // 생성자 / 소멸자 / 정적 생성자

    /// <summary>
    /// FDialogueLineTrackEditor 생성자
    /// <param name="InSequencer">시퀀서 참조</param>
    /// </summary>
    explicit FDialogueLineTrackEditor(TSharedRef<ISequencer> InSequencer);

    /// <summary>
    /// FDialogueLineTrackEditor 소멸자
    /// </summary>
    virtual ~FDialogueLineTrackEditor() override;

    /// <summary>
    /// 트랙 에디터 생성 함수
    /// <param name="Sequencer">시퀀서 참조</param>
    /// <returns>트랙 에디터 인스턴스</returns>
    /// </summary>
    static TSharedRef<ISequencerTrackEditor> CreateTrackEditor(TSharedRef<ISequencer> Sequencer);

public: // FMovieSceneTrackEditor

    /// <summary>
    /// 지원하는 트랙 타입인지 확인
    /// <param name="TrackClass">트랙 클래스</param>
    /// <returns>지원 여부</returns>
    /// </summary>
    virtual bool SupportsType(TSubclassOf<class UMovieSceneTrack> TrackClass) const override;

    /// <summary>
    /// 트랙 추가
    /// <param name="FocusedMovieScene">포커스된 무비씬</param>
    /// <param name="ObjectHandle">오브젝트 핸들</param>
    /// <param name="TrackClass">트랙 클래스</param>
    /// <param name="UniqueTypeName">유니크 타입 이름</param>
    /// <returns>생성된 트랙</returns>
    /// </summary>
    virtual UMovieSceneTrack* AddTrack(UMovieScene* FocusedMovieScene,
                                       const FGuid& ObjectHandle,
                                       TSubclassOf<class UMovieSceneTrack> TrackClass,
                                       FName UniqueTypeName) override;

    /// <summary>
    /// 트랙 추가 메뉴 빌드
    /// <param name="MenuBuilder">메뉴 빌더</param>
    /// </summary>
    virtual void BuildAddTrackMenu(FMenuBuilder& MenuBuilder) override;

    /// <summary>
    /// 아웃라이너 편집 위젯 빌드
    /// <param name="ObjectBinding">오브젝트 바인딩</param>
    /// <param name="Track">트랙</param>
    /// <param name="Params">위젯 파라미터</param>
    /// <returns>생성된 위젯</returns>
    /// </summary>
    virtual TSharedPtr<SWidget> BuildOutlinerEditWidget(const FGuid& ObjectBinding,
                                                        UMovieSceneTrack* Track,
                                                        const FBuildEditWidgetParams& Params) override;

    void PopulateMenu_AddSection(FMenuBuilder& MenuBuilder,
                                 const FName DisplayName,
                                 int32 RowIndex,
                                 UMovieSceneTrack* Track,
                                 TWeakPtr<ISequencer> InSequencer);

public: //컨텍스트 메뉴
    virtual TSharedRef<ISequencerSection> MakeSectionInterface(UMovieSceneSection& SectionObject,
                                                               UMovieSceneTrack& Track,
                                                               FGuid ObjectBinding) override;
};

struct FReferencingDialogueLineSearchResult
{
    FString CharacterName;
    FString DialogueLine;
};

class FDialogueLineSequencerSection : public FSequencerSection,
                                      public TSharedFromThis<FDialogueLineSequencerSection>
{
public: // 생성자
    FDialogueLineSequencerSection(UMovieSceneSection& InSection,
                                  const TWeakPtr<FMovieSceneTrackEditor>& InWeakEditor)
        : FSequencerSection(InSection), WeakEditor(InWeakEditor)
    {
    }

public: // 섹션에 대해
    virtual void BuildSectionContextMenu(FMenuBuilder& MenuBuilder,
                                         const FGuid& ObjectBinding) override;

private:
    TWeakPtr<FMovieSceneTrackEditor> WeakEditor;
    TSharedPtr<FExtender> Extender;

    /// <summary>
    /// 특정 UObject(에셋)를 UPROPERTY로 참조하고 있는 모든 에셋을 찾아 로그로 출력합니다.
    /// <br>
    /// AssetRegistry를 통해 1차 필터링 후, 실제 UPROPERTY 메모리 검사를 통해 2차로 확인합니다.
    /// <param name="InTargetAsset">참조를 추적할 대상 UObject (에셋)</param>
    /// </summary>
    TArray<FReferencingDialogueLineSearchResult> LogAllUPROPERTYReferencers(UObject* InTargetAsset);

private:
    /// <summary>
    /// "Import Dialogue Line" 메뉴 클릭 시 동작.<br>
    /// Section이 유효하면, LevelSequence의 아우터를 대상으로 UPROPERTY 참조 로그를 출력한다.
    /// <param name="Section">대상 시퀀서 섹션의 WeakPtr</param>
    /// </summary>
    void OnImportDialogueLineClicked();
};
