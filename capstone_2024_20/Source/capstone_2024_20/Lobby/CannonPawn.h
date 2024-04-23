// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CannonPawn.generated.h"

class UCannonRotationMovementComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class UArrowComponent;

UCLASS()
class CAPSTONE_2024_20_API ACannonPawn : public APawn
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* M_ShooterMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta=(AllowPrivateAccess="true"))
	UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere)
	UCannonRotationMovementComponent* RotationMovementComponent;
public:
	ACannonPawn();

protected:
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);

private:
	UPROPERTY(VisibleAnywhere)
	FRotator Velocity;

	static constexpr float RotationSpeed = 10.0f;
	
public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
