#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Common/HP.h"
#include "MyShip.generated.h"

class UEventSpawnPoint;
class UEnemySpawnPoint;
class AMyCannon;

UCLASS()
class CAPSTONE_2024_20_API AMyShip : public APawn, public IHP
{
	GENERATED_BODY()

public:
	AMyShip();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// [begin] IHP interface
	virtual int32 GetMaxHP() const override;
	virtual int32 GetCurrentHP() const override;
	virtual void SetMaxHP(const int32 NewMaxHP) override;
	virtual void SetCurrentHP(const int32 NewCurrentHP) override;
	virtual void Heal(const int32 HealAmount) override;
	virtual void Damage(const int32 DamageAmount) override;
	virtual void Die() override;
	// [end] IHP interface
	
	UPROPERTY(Category=Character, VisibleAnywhere)
    UStaticMeshComponent* M_MeshComponent;
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SetShipLocation(FVector NewLoc);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void UpgradeMoveSpeed();
	void UpgradeHandling();
	void UpgradeCannonAttack();
	
	float GetMoveSpeed() const;
	float GetRotationAcceleration() const;

	UPROPERTY(Replicated)
	FRotator TargetRotation;

	UPROPERTY(Replicated)
	float MoveSpeed = 600.0f;

	float RotationAcceleration = 1.0f;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	AActor* Camera_Character;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	AActor* Camera_Ship;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	AActor* Camera_Telescope;

	UFUNCTION(BlueprintPure)
	float GetHPPercent() const;

	TArray<UEnemySpawnPoint*> GetEnemySpawnPoints() const;

private:
	void FindMyCannons();
	void FindEnemySpawnPoints();
	void FindEventSpawnPoints();
	
	// [begin] IHP interface
	int32 MaxHP = 0;
	int32 CurrentHP = 0;
	// [end] IHP interface

	TArray<AMyCannon*> MyCannons;
	TArray<UEnemySpawnPoint*> EnemySpawnPoints;
	TArray<UEventSpawnPoint*> EventSpawnPoints;
};
