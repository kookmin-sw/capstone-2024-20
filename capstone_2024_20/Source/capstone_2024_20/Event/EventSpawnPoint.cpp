// ReSharper disable CppParameterMayBeConst
#include "EventSpawnPoint.h"

UEventSpawnPoint::UEventSpawnPoint()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEventSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
}

void UEventSpawnPoint::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
