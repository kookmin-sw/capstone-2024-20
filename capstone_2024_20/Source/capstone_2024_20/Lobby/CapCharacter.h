// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ICapPawn.h"
#include "GameFramework/Character.h"
#include "CapCharacter.generated.h"

class ACapInteractionActor;
class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;

UCLASS()
class CAPSTONE_2024_20_API ACapCharacter : public ACharacter, public ICapPawn
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere)
	UInputAction* InteractAction;
public:
	ACapCharacter();
	FORCEINLINE virtual UInputMappingContext* GetMappingContext() override
	{
		return DefaultMappingContext;
	};

protected:
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);
	void Interact();
	void InteractCancel();
public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;


	ACapInteractionActor* CapInteractionActor;
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
private:
	void InitMovement();
};
