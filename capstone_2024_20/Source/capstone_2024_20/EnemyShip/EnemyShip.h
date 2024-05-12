#pragma once

#include "capstone_2024_20/MyShip.h"
#include "../01_Network/ReplicatedActor.h"
#include "../Common/HP.h"
#include "Components/ArrowComponent.h"
#include "EnemyShip.generated.h"

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
	virtual void Die() override;
	
	void LookAtMyShip(const AMyShip* MyShip);
	void FireCannon(const float DeltaTime);
	AEnemy* SpawnEnemy(AActor* MyShip, const float DeltaTime) const;

	bool CanSpawnEnemy() const;

	bool bCamSpawnEnemy = false;

	FEnemyShipDie EnemyShipDieDelegate;

private:
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_LookAtMyShip(const AMyShip* MyShip);
	
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_FireCannon();
	
	inline static float SpawnEnemyTimer = 0.0f;
	inline static float FireCannonTimer = 0.0f;

	UPROPERTY()
	UArrowComponent* ProjectileSpawnPoint;

	// 발사체의 클래스를 설정하기 위한 프로퍼티
	TSubclassOf<class AEnemyShipCannonBall> ProjectileClass;

	UPROPERTY()
	UParticleSystem* FireEffect;

	UPROPERTY()
	USoundCue* CannonSoundCue;
};
