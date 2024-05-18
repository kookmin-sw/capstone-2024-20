﻿#pragma once

#include "CoreMinimal.h"
#include "capstone_2024_20/01_Network/ReplicatedActor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "EnemyShipCannonBall.generated.h"

UCLASS()
class CAPSTONE_2024_20_API AEnemyShipCannonBall : public AReplicatedActor
{
	GENERATED_BODY()

public:
	AEnemyShipCannonBall();
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void DestroyWithDelay();
	
	UPROPERTY()
	UStaticMeshComponent* StaticMesh;

	UPROPERTY()
	UProjectileMovementComponent* ProjectileMovement;
	
	UPROPERTY()
	UParticleSystem* WaterSplashEffect;

	const float DestroyDelayTime = 1.0f;
	const FVector WaterSplashEffectScale = FVector(3.0f, 3.0f, 3.0f);

	const int Damage = 1;
};
