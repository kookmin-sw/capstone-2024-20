// Fill out your copyright notice in the Description page of Project Settings.


#include "CapInteractionActor.h"

#include "CapCharacter.h"
#include "LobbyInteractionWidgetComponent.h"
#include "../Common/InteractionWidgetComponent.h"


ACapInteractionActor::ACapInteractionActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACapInteractionActor::BeginPlay()
{
	Super::BeginPlay();
}

float ACapInteractionActor::GetLongInteractionThreshold()
{
	return LongInteractionThreshold;
}

void ACapInteractionActor::InteractionEnter()
{
	Super::InteractionEnter();
	GetWorldTimerManager().SetTimer(EnterTimerHandle, this, &ThisClass::InteractionLongEnter, LongInteractionThreshold);

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (PlayerController)
	{
		CapCharacter = Cast<ACapCharacter>(PlayerController->GetCharacter());
	}
}

void ACapInteractionActor::InteractionExit()
{
	Super::InteractionExit();
	GetWorldTimerManager().ClearTimer(EnterTimerHandle);
}

void ACapInteractionActor::InteractionLongEnter()
{
	CapCharacter->InteractionWidgetComponent->StopProgressBar();
}
