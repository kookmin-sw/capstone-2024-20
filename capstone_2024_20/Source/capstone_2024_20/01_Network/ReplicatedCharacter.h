#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ReplicatedCharacter.generated.h"

UCLASS()
class CAPSTONE_2024_20_API AReplicatedCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AReplicatedCharacter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated = OnRep_Location)
	FVector Location;

	UPROPERTY(ReplicatedUsing = OnRep_Rotation)
	FRotator Rotation;

	UFUNCTION()
	void OnRep_Location();
	UFUNCTION()
	void OnRep_Rotation();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
