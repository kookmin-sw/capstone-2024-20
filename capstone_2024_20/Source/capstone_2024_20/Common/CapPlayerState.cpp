// Fill out your copyright notice in the Description page of Project Settings.


#include "CapPlayerState.h"

#include "CapGameState.h"


ACapPlayerState::ACapPlayerState()
{
	bReplicates = true;
}

void ACapPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetGameState<ACapGameState>()->LocalReadyDelegate.AddUObject(
		this, &ThisClass::ServerRPC_StartReady);
}

void ACapPlayerState::ServerRPC_StartReady_Implementation()
{
	GetWorld()->GetGameState<ACapGameState>()->Ready();
}
