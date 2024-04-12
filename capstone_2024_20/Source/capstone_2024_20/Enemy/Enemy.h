﻿#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Common/HP.h"
#include "Enemy.generated.h"

class AMyCharacter;

UCLASS()
class CAPSTONE_2024_20_API AEnemy : public AActor, public IHP
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void BeginPlay() override;
	void MoveToMyCharacter(const AMyCharacter* MyCharacter);
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;
};
