// Fill out your copyright notice in the Description page of Project Settings.


#include "CapGameState.h"

#include "Net/UnrealNetwork.h"

ACapGameState::ACapGameState()
{
	RoomState = FRoomState::EnteringMap;
}

void ACapGameState::OnRep_RoomState()
{
}


void ACapGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME(ACapGameState, RoomState);
}