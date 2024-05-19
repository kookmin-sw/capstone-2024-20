

#include "CapPawn.h"


ACapPawn::ACapPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

void ACapPawn::BeginPlay()
{
	Super::BeginPlay();
}

void ACapPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

