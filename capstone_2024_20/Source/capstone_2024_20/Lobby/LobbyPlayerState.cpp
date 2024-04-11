// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerState.h"
#include "Net/UnrealNetwork.h"

void ALobbyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerState, bIsReady);
	DOREPLIFETIME_CONDITION(ALobbyPlayerState, PlayerNumber, COND_InitialOnly);
}

void ALobbyPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		GEngine->AddOnScreenDebugMessage(-1,
		                                 60.0f, FColor::Emerald,
		                                 TEXT("Has AUtority"));
	}
}

void ALobbyPlayerState::SetReady()
{
	if(HasAuthority())
	{
		return;
	}
	
	if (bIsCanReady == true)
	{
		bIsCanReady = false;
		Server_SetReady(!bIsReady);
	}
}

void ALobbyPlayerState::OnRep_IsReady()
{	
	bIsCanReady = true;
	OnIsReadyChanged.Broadcast();
}

void ALobbyPlayerState::SetInitPlayerNumber(int Number)
{
	PlayerNumber = Number;
}


void ALobbyPlayerState::Server_SetReady_Implementation(bool IsReady)
{
	bIsReady = IsReady;

	if (HasAuthority())
	{
		OnRep_IsReady();
	}
}

bool ALobbyPlayerState::IsReady() const
{
	if(PlayerNumber == 1)
	{
		return true;
	}
	
	return bIsReady;
}
