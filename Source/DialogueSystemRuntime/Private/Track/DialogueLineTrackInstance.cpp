// Fill out your copyright notice in the Description page of Project Settings.


#include "Track/DialogueLineTrackInstance.h"

#include "Component/DialogueUIComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Section/DialogueLineSection.h"

UDialogueLineTrackInstance::UDialogueLineTrackInstance()
{
}

void UDialogueLineTrackInstance::OnInputAdded(const FMovieSceneTrackInstanceInput& InInput)
{
    // TODO UBlueprintFunctionLibrary::GetSubtitleUIComponent()로 분리
    if ( const auto WorldType = GetWorld()->WorldType.GetValue();
        !(WorldType == EWorldType::Type::Editor
          || WorldType == EWorldType::Type::EditorPreview) )
    {
        const auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        const auto PlayerCharacter  = PlayerController ? PlayerController->GetCharacter() : nullptr;
        const auto Component        = PlayerCharacter ? PlayerCharacter->GetComponentByClass(UDialogueUIComponent::StaticClass()) : nullptr;
        if ( !Component )
        {
            return;
        }
        const auto SubtitleUIComponent = Cast<UDialogueUIComponent>(Component);

        checkf(SubtitleUIComponent, TEXT("SubtitleComponent가 유효하지 않습니다. 캐릭터에 컴포넌트가 있는지 확인하세요."))

        if ( const auto Section = Cast<UDialogueLineSection>(InInput.Section) )
        {
            const auto Param = FSubtitleParam::CreateSubtitleWithCharacterName(Section->CharacterName, Section->DialogueLine);
            SubtitleUIComponent->SetSubtitle(Param);
        }
    }
}

void UDialogueLineTrackInstance::OnInputRemoved(const FMovieSceneTrackInstanceInput& InInput)
{
    if ( const auto WorldType = GetWorld()->WorldType.GetValue();
        !(WorldType == EWorldType::Type::Editor
          || WorldType == EWorldType::Type::EditorPreview) )
    {
        const auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        const auto PlayerCharacter  = PlayerController ? PlayerController->GetCharacter() : nullptr;
        const auto Component        = PlayerCharacter ? PlayerCharacter->GetComponentByClass(UDialogueUIComponent::StaticClass()) : nullptr;

        if ( !Component )
        {
            return;
        }

        const auto SubtitleUIComponent = Cast<UDialogueUIComponent>(Component);

        checkf(SubtitleUIComponent, TEXT("SubtitleComponent가 유효하지 않습니다. 캐릭터에 컴포넌트가 있는지 확인하세요."))

        SubtitleUIComponent->EmptySubtitle();
        SubtitleUIComponent->HideSubtitle();
    }
}
