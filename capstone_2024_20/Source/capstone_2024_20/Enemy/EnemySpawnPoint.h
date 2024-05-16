// Class for the enemy spawn point, basically do nothing but to be a reference point for the enemy to spawn

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "EnemySpawnPoint.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CAPSTONE_2024_20_API UEnemySpawnPoint : public USceneComponent
{
	GENERATED_BODY()

public:
	UEnemySpawnPoint();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
