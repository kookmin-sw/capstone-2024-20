#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../MyObject.h"
#include "Components/BoxComponent.h"
#include "Event.generated.h"

UCLASS()
class CAPSTONE_2024_20_API AEvent : public AMyObject
{
	GENERATED_BODY()

public:
	AEvent();

	virtual void BeginPlay() override;
	virtual void Operate() override;

private:
	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* Particle;

	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxComponent;
};
