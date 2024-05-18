#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../MyObject.h"
#include "Components/BoxComponent.h"
#include "Event.generated.h"

class AMyShip;
class AEvent;

DECLARE_DELEGATE_OneParam(FEventOperateDelegate, AEvent*);

UCLASS()
class CAPSTONE_2024_20_API AEvent : public AMyObject
{
	GENERATED_BODY()

public:
	AEvent();

	virtual void BeginPlay() override;
	virtual void Operate() override;

	void DamageMyShip(AMyShip* MyShip);
	void ReduceCurrentDamageCooldown(const float DeltaTime);
	
	bool CanDamageMyShip() const;
	
	FEventOperateDelegate EventOperateDelegate;

private:
	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* Particle;

	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxComponent;

	FVector AdjustedScale = FVector(0.4f, 0.4f, 0.4f);

	const int Damage = 1;
	const float DamageCooldown = 10.0f;
	float CurrentDamageCooldown = 10.0f;
};
