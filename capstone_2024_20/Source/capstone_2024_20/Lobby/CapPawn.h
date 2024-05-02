// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ICapPawn.h"
#include "GameFramework/Pawn.h"
#include "CapPawn.generated.h"

class UInputAction;

UCLASS()
class CAPSTONE_2024_20_API ACapPawn : public APawn, public IMappingContextGetter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere)
	UInputAction* MoveAction;

public:
	ACapPawn();

	FORCEINLINE virtual UInputMappingContext* GetMappingContext() override
	{
		return DefaultMappingContext;
	};

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
