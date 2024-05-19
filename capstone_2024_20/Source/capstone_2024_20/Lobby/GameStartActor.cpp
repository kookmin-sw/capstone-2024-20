// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStartActor.h"

#include "CapCharacter.h"
#include "LevelSequencePlayer.h"
#include "LobbyGameMode.h"


AGameStartActor::AGameStartActor()
{
	PrimaryActorTick.bCanEverTick = true;
	LongInteractionThreshold = 2.0f;

	ExplainText = TEXT("게임시작");
}

void AGameStartActor::BeginPlay()
{
	Super::BeginPlay();
}

void AGameStartActor::InteractionEnter()
{
	Super::InteractionEnter();
}

void AGameStartActor::InteractionLongEnter()
{
	Super::InteractionLongEnter();
	ALobbyGameMode* GameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (HasAuthority() == true && GameMode->IsReadyAllPlayer())
	{
		FTimerHandle TImerHandle;
		GetWorld()->GetTimerManager().SetTimer(TImerHandle, this, &ThisClass::GameStart, 22.0f);
		Multicast_PlaySequence();
	}
}

void AGameStartActor::InteractionExit()
{
	Super::InteractionExit();
}

void AGameStartActor::GameStart()
{
	ALobbyGameMode* GameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	GameMode->GameStart();
}

void AGameStartActor::Multicast_PlaySequence_Implementation()
{
	FMovieSceneSequencePlaybackSettings PlaybackSettings;
	ALevelSequenceActor* LevelSequenceActor;
	LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld(),
		LevelSequence,
		PlaybackSettings,
		LevelSequenceActor
	);


	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (PlayerController)
	{
		ACapCharacter* ClientCharacter = Cast<ACapCharacter>(PlayerController->GetCharacter());
		ClientCharacter->SetVisibleWigetWithBool(false);
	}

	
	LevelSequencePlayer->Play();
}
