#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionActor.generated.h"

UCLASS()
class CAPSTONE_2024_20_API AInteractionActor : public AActor
{
	GENERATED_BODY()

public:
	AInteractionActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
