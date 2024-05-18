#pragma once

#include "CoreMinimal.h"
#include "MyObject.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "MyCannon.generated.h"

UCLASS()
class CAPSTONE_2024_20_API AMyCannon : public AMyObject
{
	GENERATED_BODY()
	
public:	
	AMyCannon();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, Category="Sound")
	USoundCue* CannonSoundCue;

private:
	UPROPERTY(Replicated)
	bool IsLoad = false;
	int AttackDamage = 1;
	
public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCannonWidgetComponent* WidgetComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* M_ShooterMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<class ACannonBall> ProjectileClass;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Effects")
	UParticleSystem* FireEffect;

	void TriggerEffects() const;
	
	UFUNCTION()
	FVector GetCannonSpawnLocation() const;
	
	UFUNCTION()
	FRotator GetCannonSpawnRotation() const;

	UFUNCTION()
	void FireCannon();

	UFUNCTION(Server, Reliable)
	void ServerRPC_FireCannon();

	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_FireCannon();

	UFUNCTION()
	void RotateCannon(const FRotator& NewRot);

	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_RotateCannon(FRotator NewRot);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void VisibleWidget(bool b) const;
	
	bool GetIsLoad() const;
	void SetIsLoad(bool b);

	void UpgradeAttackDamage();

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	AActor* Camera_Cannon;
};
