// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ICapPawn.h"
#include "GameFramework/Character.h"
#include "CapCharacter.generated.h"

class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;

UCLASS()
class CAPSTONE_2024_20_API ACapCharacter : public ACharacter, public IMappingContextGetter
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere)
	UInputAction* MoveAction;

public:
	ACapCharacter();
	FORCEINLINE virtual UInputMappingContext* GetMappingContext() override
	{
		return DefaultMappingContext;
	};

protected:
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	void InitMovement();
};
