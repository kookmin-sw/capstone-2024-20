#include "InteractionActor.h"


AInteractionActor::AInteractionActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AInteractionActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteractionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}