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

public:
	virtual void InteractionEnter();
	virtual void InteractionExit();
};
