// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CannonRotationMovementComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CAPSTONE_2024_20_API UCannonRotationMovementComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	FRotator Velocity;

	UPROPERTY(ReplicatedUsing=On_MovementBase)
	FRotator MovementBase;
	
	UFUNCTION()
	void On_MovementBase();

	UPROPERTY(EditAnywhere)
	float RotationSpeed = 10.0f;

	UPROPERTY()
	USceneComponent* UpdatedComponent;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	UCannonRotationMovementComponent();
	
	UFUNCTION()
	virtual void SetUpdatedComponent(USceneComponent* NewUpdatedComponent);
	
	UFUNCTION()
	virtual void AddMovementRotation(FRotator WorldDirection);

	void ControlledCharacterMove();
	
	UFUNCTION(Unreliable, Server)
	void ServerToMove(FRotator Rotation);
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
