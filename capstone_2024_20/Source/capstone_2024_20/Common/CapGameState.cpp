// Fill out your copyright notice in the Description page of Project Settings.


#include "CapGameState.h"

#include "Net/UnrealNetwork.h"

ACapGameState::ACapGameState()
{
	RoomState = FRoomState::None;
}

void ACapGameState::HandleNone()
{
}

void ACapGameState::HandleJoiningUser()
{
}

void ACapGameState::HandleAllReady()
{
}

void ACapGameState::HandleGameStart()
{
}

void ACapGameState::OnRep_ReplicatedHasBegunPlay()
{
	Super::OnRep_ReplicatedHasBegunPlay();
	HandleReady();
}

void ACapGameState::HandleBeginPlay()
{
	Super::HandleBeginPlay();
	HandleReady();
}

void ACapGameState::HandleReady()
{
	ServerRPC_Ready();
}

void ACapGameState::ServerRPC_Ready_Implementation()
{
}

void ACapGameState::OnRep_RoomState()
{
	if(RoomState == FRoomState::None)
	{
		HandleNone();
	}
	else if(RoomState == FRoomState::JoiningUser)
	{
		HandleJoiningUser();
	}
	else if(RoomState == FRoomState::AllReady)
	{
		HandleAllReady();
	}
	else if(RoomState == FRoomState::GameStart)
	{
		HandleGameStart();
	}
}

void ACapGameState::SetRoomState(const FName NewRoomState)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		RoomState = NewRoomState;

		OnRep_RoomState();
	}
}


void ACapGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME(ACapGameState, RoomState);
}
