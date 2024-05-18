// A class for the spawn point of the event, basically do nothing

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "EventSpawnPoint.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CAPSTONE_2024_20_API UEventSpawnPoint : public USceneComponent
{
	GENERATED_BODY()

public:
	UEventSpawnPoint();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
