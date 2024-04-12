#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacle.generated.h"

UCLASS()
class CAPSTONE_2024_20_API AObstacle : public AActor
{
	GENERATED_BODY()

public:
	AObstacle();

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent;
};
