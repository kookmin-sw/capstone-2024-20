﻿#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SailingSystem.generated.h"

class AMyShip;
class AEnemyShip;
class AEnemy;
class AEvent;
class UTrigger;
class AMyCharacter;

UCLASS()
class CAPSTONE_2024_20_API ASailingSystem : public AActor
{
	GENERATED_BODY()

public:
	ASailingSystem();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SpawnEnemyShip();
	void SpawnEvent();
	
	void EarnCurrency(int32 Amount);
	void UseCurrency(int32 Amount);

	void UpgradeMyShip() const;

	void SetMyShip();
	void SetMyCharacters();

private:
	inline static float SpawnEnemyShipTimer = 0.0f;
	TArray<AEnemyShip*> EnemyShips;

	TArray<AEnemy*> Enemies;

	inline static float SpawnEventTimer = 0.0f;
	TArray<AEvent*> Events;

	TArray<AMyCharacter*> MyCharacters;

	UPROPERTY()
	UTrigger* ClearTrigger;

	UPROPERTY()
	UTrigger* GameOverTrigger;

	UPROPERTY()
	AMyShip* MyShip;

	int32 Currency = 0;
};
