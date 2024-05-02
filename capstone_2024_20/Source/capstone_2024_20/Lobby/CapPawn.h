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
	
public:
	ACapPawn();

	FORCEINLINE virtual UInputMappingContext* GetMappingContext() override
	{
		return DefaultMappingContext;
	};

protected:
	UPROPERTY(EditAnywhere)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere)
	UInputAction* MoveAction;

	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
