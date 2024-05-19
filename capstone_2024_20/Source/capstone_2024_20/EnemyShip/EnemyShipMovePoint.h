#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "EnemyShipMovePoint.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CAPSTONE_2024_20_API UEnemyShipMovePoint : public USceneComponent
{
	GENERATED_BODY()

public:
	UEnemyShipMovePoint();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
