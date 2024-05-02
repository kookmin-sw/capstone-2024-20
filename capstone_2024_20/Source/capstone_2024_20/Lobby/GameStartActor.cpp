// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStartActor.h"

#include "LobbyGameMode.h"


AGameStartActor::AGameStartActor()
{
	PrimaryActorTick.bCanEverTick = true;
	LongInteractionThreshold = 2.0f;
}

void AGameStartActor::BeginPlay()
{
	Super::BeginPlay();
}

void AGameStartActor::InteractionLongEnter()
{
	Super::InteractionLongEnter();
	if(HasAuthority() == true)
	{
		ALobbyGameMode* GameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
		GameMode->GameStart();
	}
}

