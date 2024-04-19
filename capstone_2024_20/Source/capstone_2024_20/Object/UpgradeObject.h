#pragma once

#include "CoreMinimal.h"
#include "../MyObject.h"
#include "GameFramework/Actor.h"
#include "UpgradeObject.generated.h"

class UBoxComponent;

UCLASS()
class CAPSTONE_2024_20_API AUpgradeObject : public AMyObject
{
	GENERATED_BODY()

public:
	AUpgradeObject();

	virtual void BeginPlay() override;
	virtual void Operate() override;

private:
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxComponent;
};
