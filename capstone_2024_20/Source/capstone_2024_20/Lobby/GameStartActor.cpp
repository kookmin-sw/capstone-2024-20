// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStartActor.h"

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
	if(HasAuthority() == true)
	{
		ALobbyGameMode* GameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
		GameMode->GameStart();
	}
}

void AGameStartActor::InteractionExit()
{
	Super::InteractionExit();
}

