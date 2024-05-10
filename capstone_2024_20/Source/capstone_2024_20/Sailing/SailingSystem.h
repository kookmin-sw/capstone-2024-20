#pragma once

#include "CoreMinimal.h"
#include "capstone_2024_20/Enemy/Enemy.h"
#include "GameFramework/Actor.h"
#include "SailingSystem.generated.h"

class AMyShip;
class AEnemyShip;
class AEnemy;
class AEvent;
class UTrigger;
class AMyCharacter;
class UMap;

UCLASS()
class CAPSTONE_2024_20_API ASailingSystem : public AActor
{
	GENERATED_BODY()

public:
	ASailingSystem();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void OnEnemyDie(AEnemy* Enemy);
	void OnEnemyShipDie(AEnemyShip* EnemyShip);

	void CreateMap();
	void CreateObstacles() const;
	
	void SpawnEvent();

	void CalculateEnemyInAttackRange();
	
	void EarnCurrency(int32 Amount);
	void UseCurrency(int32 Amount);

	void UpgradeMyShip() const;

	float GetElapsedTime() const;
	void SetMyShip();
	void SetMyCharacters();

private:
	UPROPERTY()
	UMap* Map;
	
	TArray<AEnemyShip*> EnemyShips;

	UPROPERTY(Replicated)
	TArray<AEnemy*> Enemies;

	inline static float SpawnEventTimer = 0.0f;
	TArray<AEvent*> Events;

	UPROPERTY(Replicated)
	TArray<AMyCharacter*> MyCharacters;

	UPROPERTY()
	UTrigger* ClearTrigger;

	UPROPERTY()
	UTrigger* GameOverTrigger;

	UPROPERTY()
	AMyShip* MyShip;

	int32 Currency = 0;

	bool bIsClear = false;
	float ElapsedTime = 0;
};
