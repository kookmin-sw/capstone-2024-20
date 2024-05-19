// ReSharper disable CppParameterMayBeConst
#include "EnemyShipMovePoint.h"

UEnemyShipMovePoint::UEnemyShipMovePoint()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEnemyShipMovePoint::BeginPlay()
{
	Super::BeginPlay();
}

void UEnemyShipMovePoint::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
