// Fill out your copyright notice in the Description page of Project Settings.


#include "CapGameState.h"

#include "CapPlayerState.h"
#include "Net/UnrealNetwork.h"

ACapGameState::ACapGameState()
{
	RoomState = FRoomState::None;
	NumReadyPlayers = 0;
	bReplicates = true;
}

void ACapGameState::HandleNone()
{
	NoneDelegate.Broadcast();
}

void ACapGameState::HandleJoiningUser()
{
	JoiningUserDelegate.Broadcast();
}

void ACapGameState::HandleAllReady()
{
	AllReadyDelegate.Broadcast();
}

void ACapGameState::HandleGameStart()
{
	GameStartDelegate.Broadcast();
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
	LocalReadyDelegate.Broadcast();
}

void ACapGameState::Ready()
{
	if (HasAuthority() == false)
		return;
	
	SetRoomState(FRoomState::AllReady);
}


void ACapGameState::OnRep_RoomState()
{
	if (RoomState == FRoomState::None)
	{
		HandleNone();
	}
	else if (RoomState == FRoomState::JoiningUser)
	{
		HandleJoiningUser();
	}
	else if (RoomState == FRoomState::AllReady)
	{
		HandleAllReady();
	}
	else if (RoomState == FRoomState::GameStart)
	{
		HandleGameStart();
	}
}

void ACapGameState::SetRoomState(const FName NewRoomState)
{
	if (RoomState == NewRoomState)
		return;

	if (GetLocalRole() == ROLE_Authority)
	{
		RoomState = NewRoomState;

		OnRep_RoomState();
	}
}


void ACapGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACapGameState, RoomState);
}
