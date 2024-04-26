// Fill out your copyright notice in the Description page of Project Settings.


#include "CapGameMode.h"
#include "CapGameState.h"
#include "CapPlayerState.h"

ACapGameMode::ACapGameMode()
{
	GameStateClass = ACapGameState::StaticClass();
	PlayerStateClass = ACapPlayerState::StaticClass();
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
