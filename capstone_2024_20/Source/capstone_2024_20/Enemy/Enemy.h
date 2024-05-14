#pragma once

#include "CoreMinimal.h"
#include "NavigationInvokerComponent.h"
#include "../01_Network/ReplicatedCharacter.h"
#include "../Common/HP.h"
#include "Enemy.generated.h"

class AMyCharacter;
class AEnemy;

DECLARE_DELEGATE_OneParam(FEnemyDie, AEnemy*);

UCLASS()
class CAPSTONE_2024_20_API AEnemy : public AReplicatedCharacter, public IHP
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void BeginPlay() override;

	// [begin] IHP interface
	virtual int32 GetMaxHP() const override;
	virtual int32 GetCurrentHP() const override;
	virtual void SetMaxHP(const int32 NewMaxHP) override;
	virtual void SetCurrentHP(const int32 NewCurrentHP) override;
	virtual void Heal(const int32 HealAmount) override;
	virtual void Damage(const int32 DamageAmount) override;
	virtual void Die() override;
	// [end] IHP interface
	
	void MoveToMyCharacter(const AMyCharacter* MyCharacter);
	void Attack(AMyCharacter* MyCharacter);

	UFUNCTION(Server, Reliable)
	void ServerRPC_Attack(AMyCharacter* MyCharacter);
	
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_Attack(AMyCharacter* MyCharacter);

	void ReduceCurrentAttackCooldown(float DeltaTime);

	float GetDistanceToMyCharacter() const;
	bool CanMove() const;
	bool CanAttack() const;
	
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY()
	UNavigationInvokerComponent* NavInvoker;

	FEnemyDie EnemyDieDelegate;

private:
	// [begin] IHP interface
	int32 MaxHP = 0;
	int32 CurrentHP = 0;
	// [end] IHP interface
	
	const float MoveSpeed = 100.0f;
	const float DistanceToMyCharacter = 200.0f;
	const float AttackCooldown = 5.0f;
	float CurrentAttackCooldown = 0.0f;
};
