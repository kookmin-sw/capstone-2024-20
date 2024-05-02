// Fill out your copyright notice in the Description page of Project Settings.


#include "CapInteractionActor.h"


ACapInteractionActor::ACapInteractionActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACapInteractionActor::BeginPlay()
{
	Super::BeginPlay();
}

void ACapInteractionActor::InteractionEnter()
{
	Super::InteractionEnter();
	GetWorldTimerManager().SetTimer(EnterTimerHandle, this, &ThisClass::InteractionLongEnter, LongInteractionThreshold);
}

void ACapInteractionActor::InteractionExit()
{
	Super::InteractionExit();
	GetWorldTimerManager().ClearTimer(EnterTimerHandle);
}

void ACapInteractionActor::InteractionLongEnter()
{
}
