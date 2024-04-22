// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CapController.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONE_2024_20_API ACapController : public APlayerController
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

private:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnRep_Pawn() override;

	void RefreshMappingContext(APawn* InPawn) const;
};
