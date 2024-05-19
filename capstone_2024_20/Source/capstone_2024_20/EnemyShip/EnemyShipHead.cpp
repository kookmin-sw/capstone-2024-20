// ReSharper disable CppParameterMayBeConst
#include "EnemyShipHead.h"

UEnemyShipHead::UEnemyShipHead()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEnemyShipHead::BeginPlay()
{
	Super::BeginPlay();
}

void UEnemyShipHead::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

