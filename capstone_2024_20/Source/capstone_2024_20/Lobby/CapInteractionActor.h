#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CapInteractionActor.generated.h"

UCLASS()
class CAPSTONE_2024_20_API ACapInteractionActor : public AActor
{
	GENERATED_BODY()

public:
	ACapInteractionActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
