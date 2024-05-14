#include "LobbyCameraActor.h"


ALobbyCameraActor::ALobbyCameraActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALobbyCameraActor::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetFirstPlayerController()->SetViewTarget(this);
}

void ALobbyCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

