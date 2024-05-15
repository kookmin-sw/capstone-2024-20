#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "01_Network/ReplicatedActor.h"
#include "CannonBall.generated.h"

UCLASS()
class CAPSTONE_2024_20_API ACannonBall : public AReplicatedActor
{
	GENERATED_BODY()
	
public:	
	ACannonBall();

protected:
	virtual void BeginPlay() override;

private:
	float DestroyDelayTime = 1.0f;
	int32 Damage = 1;
	
public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void DestroyDelay();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Effects")
	UParticleSystem* WaterSplashEffect;

	void SetDamage(int32 Dmg);
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_OnHit(const FHitResult& Hit);
};
