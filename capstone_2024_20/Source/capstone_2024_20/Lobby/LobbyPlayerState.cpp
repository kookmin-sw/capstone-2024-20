// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerState.h"
#include "Net/UnrealNetwork.h"

void ALobbyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerState, bIsReady);
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
	if (bIsCanReady == true)
	{
		bIsCanReady = false;
		Server_SetReady(!bIsReady);
	}
}

void ALobbyPlayerState::OnRep_IsReady()
{
	bIsCanReady = true;
	OnIsReadyChanged.Execute();
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
	return bIsReady;
}
