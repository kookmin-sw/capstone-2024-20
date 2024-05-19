#pragma once

#include "capstone_2024_20/MyShip.h"
#include "../01_Network/ReplicatedActor.h"
#include "../Common/HP.h"
#include "Components/ArrowComponent.h"
// 패키징 문제로 인해, SoundCue 헤더 파일 include 필요
// ReSharper disable once CppUnusedIncludeDirective
#include "Sound/SoundCue.h"
#include "EnemyShip.generated.h"

// ReSharper disable once IdentifierTypo
class AMyIngameHUD;
class UNavigationPath;
class AMyShip;
class AEnemy;
class AEnemyShip;
class UEnemySpawnPoint;

DECLARE_DELEGATE_OneParam(FEnemyShipDie, AEnemyShip*);

UCLASS()
class AEnemyShip : public AReplicatedActor, public IHP
{
	GENERATED_BODY()
	
public:
	AEnemyShip();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// [begin] IHP interface
	virtual int32 GetMaxHP() const override;
	virtual int32 GetCurrentHP() const override;
	virtual void SetMaxHP(const int32 NewMaxHP) override;
	virtual void SetCurrentHP(const int32 NewCurrentHP) override;
	virtual void Heal(const int32 HealAmount) override;
	virtual void Damage(const int32 DamageAmount) override;
	virtual void Die() override;
	// [end] IHP interface

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPC_Damage();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_Die();

	void MoveToMyShip(const AMyShip* MyShip, const float DeltaTime);
	void LookAtMyShip(const AMyShip* MyShip);
	void FireCannon(const float DeltaTime);
	AEnemy* SpawnEnemy(AMyShip* MyShip);

	bool CanMove(const AMyShip* MyShip) const;
	bool CanSpawnEnemy(const AMyShip* MyShip) const;
	bool CanFireCannon() const;

	void ReduceSpawnEnemyCooldown(const float DeltaTime);

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

	// Returns random enemy spawn point from two nearest enemy spawn points
	UEnemySpawnPoint* GetEnemySpawnPointToSpawn(const AMyShip* MyShip) const;
	
	// [begin] IHP interface
	UPROPERTY(Replicated)
	int32 MaxHP = 0;

	UPROPERTY(Replicated)
	int32 CurrentHP = 0;
	// [end] IHP interface
	
	inline static float FireCannonTimer = 0.0f;

	inline static float DistanceToMyShip = 7000.0f;
	inline static float MoveSpeed = 400.0f;

	const float SpawnEnemyCooldown = 5.0f;
	float CurrentSpawnEnemyCooldown = 0.0f;
	
	UPROPERTY()
	UArrowComponent* ProjectileSpawnPoint;
	
	TSubclassOf<class AEnemyShipCannonBall> ProjectileClass;

	UPROPERTY()
	UParticleSystem* FireEffect;

	UPROPERTY()
	USoundCue* CannonSoundCue;

	UPROPERTY()
	AMyIngameHUD* MyInGameHUD;
};
