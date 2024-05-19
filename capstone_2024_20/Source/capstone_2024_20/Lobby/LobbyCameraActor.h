#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "LobbyCameraActor.generated.h"

UCLASS()
class CAPSTONE_2024_20_API ALobbyCameraActor : public ACameraActor
{
	GENERATED_BODY()

public:
	ALobbyCameraActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
