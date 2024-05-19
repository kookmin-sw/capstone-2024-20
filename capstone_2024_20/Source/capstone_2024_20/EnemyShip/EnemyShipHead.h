#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "EnemyShipHead.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CAPSTONE_2024_20_API UEnemyShipHead : public USceneComponent
{
	GENERATED_BODY()

public:
	UEnemyShipHead();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
