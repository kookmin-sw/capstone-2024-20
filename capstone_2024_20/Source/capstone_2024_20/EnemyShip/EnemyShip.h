#pragma once

#include "capstone_2024_20/MyShip.h"
#include "../01_Network/ReplicatedActor.h"
#include "../Common/HP.h"
#include "Components/ArrowComponent.h"
#include "EnemyShip.generated.h"

class UNavigationPath;
class AMyShip;
class AEnemy;
class AEnemyShip;

DECLARE_DELEGATE_OneParam(FEnemyShipDie, AEnemyShip*);

UCLASS()
class AEnemyShip : public AReplicatedActor, public IHP
{
	GENERATED_BODY()
	
public:
	AEnemyShip();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Die() override;

	void MoveToMyShip(const AMyShip* MyShip, const float DeltaTime);
	void LookAtMyShip(const AMyShip* MyShip);
	void FireCannon(const float DeltaTime);
	AEnemy* SpawnEnemy(AActor* MyShip, const float DeltaTime) const;

	bool CanMove(const AMyShip* MyShip) const;
	bool CanSpawnEnemy() const;
	bool CanFireCannon() const;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bCanSpawnEnemy = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bCanFireCannon = true;

	FEnemyShipDie EnemyShipDieDelegate;

private:
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_LookAtMyShip(const AMyShip* MyShip);
	
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_FireCannon();
	
	inline static float SpawnEnemyTimer = 0.0f;
	inline static float FireCannonTimer = 0.0f;

	inline static float DistanceToMyShip = 7000.0f;
	inline static float MoveSpeed = 400.0f;
	
	UPROPERTY()
	UArrowComponent* ProjectileSpawnPoint;
	
	TSubclassOf<class AEnemyShipCannonBall> ProjectileClass;

	UPROPERTY()
	UParticleSystem* FireEffect;

	UPROPERTY()
	USoundCue* CannonSoundCue;
};
