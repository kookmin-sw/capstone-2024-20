#pragma once

#include "CoreMinimal.h"
#include "capstone_2024_20/Enemy/Enemy.h"
#include "../01_Network/ReplicatedActor.h"
#include "SailingSystem.generated.h"

// ReSharper disable once IdentifierTypo
class AMyIngameHUD;
class ADestination;
class AMyShip;
class AEnemyShip;
class AEnemy;
class AEvent;
class UTrigger;
class AMyCharacter;
class UMap;

UCLASS()
class CAPSTONE_2024_20_API ASailingSystem : public AReplicatedActor
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

	UFUNCTION(BlueprintPure)
	int GetCurrency() const;

	void AddDelegateToPopupUpgrade();

	void UpgradeMyShipMoveSpeed();
	void UpgradeMyShipHandling();
	void UpgradeMyShipCannonAttack();

	float GetElapsedTime() const;
	bool IsAllMyCharactersDead() const;
	bool IsReachedDestination() const;
	
	AMyShip* GetMyShip() const;
	void SetMyShip();
	void SetMyCharacters();
	void SetEnemyShips();
	void SetDestination();

	UFUNCTION(BlueprintPure)
	float DestinationProgress();

private:
	AMyCharacter* FindNearestMyCharacter(const AEnemy* Enemy) const;
	
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

	UPROPERTY()
	ADestination* Destination;

	const float DistanceToDestination = 6000.0f;
	
	UPROPERTY(Replicated)
	int32 Currency = 10000;

	const int32 UpgradeCost = 2000;

	bool bIsClear = false;
	bool bIsGameOver = false;
	float ElapsedTime = 0;

	FVector InitLocation;
	UPROPERTY(Replicated)
	float Progress = 0.0f;
	
	UPROPERTY()
	AMyIngameHUD* MyInGameHUD = nullptr;
};
