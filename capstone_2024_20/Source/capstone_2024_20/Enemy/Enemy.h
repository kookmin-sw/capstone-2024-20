#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

UCLASS()
class CAPSTONE_2024_20_API AEnemy : public AActor
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;
};
