#include "EnemySpawnPoint.h"

UEnemySpawnPoint::UEnemySpawnPoint()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEnemySpawnPoint::BeginPlay()
{
	Super::BeginPlay();
}

void UEnemySpawnPoint::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

