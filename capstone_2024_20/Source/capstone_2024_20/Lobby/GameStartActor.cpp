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

void AGameStartActor::InteractionEnter()
{
	Super::InteractionEnter();
	GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Blue,
	TEXT("InteractionEnter"));
}

void AGameStartActor::InteractionLongEnter()
{
	Super::InteractionLongEnter();
	GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Yellow,
		TEXT("InteractionLongEnter"));
	if(HasAuthority() == true)
	{
		ALobbyGameMode* GameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
		GameMode->GameStart();
	}
}

void AGameStartActor::InteractionExit()
{
	Super::InteractionExit();
	GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red,
	TEXT("InteractionLongExit"));
}

