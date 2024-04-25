// Fill out your copyright notice in the Description page of Project Settings.


#include "CapGameMode.h"
#include "CapGameState.h"

ACapGameMode::ACapGameMode()
{
	GameStateClass = ACapGameState::StaticClass();
}


void ACapGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ACapGameState* CapGameState = GetGameState<ACapGameState>();
	if(CapGameState)
	{
		CapGameState->SetRoomState(FRoomState::JoiningUser);
	}
}
