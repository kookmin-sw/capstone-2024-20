#include "CapInteractionActor.h"


ACapInteractionActor::ACapInteractionActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACapInteractionActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACapInteractionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

