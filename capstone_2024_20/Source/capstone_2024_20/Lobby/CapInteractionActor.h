// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionActor.h"
#include "CapInteractionActor.generated.h"

UCLASS()
class CAPSTONE_2024_20_API ACapInteractionActor : public AInteractionActor
{
	GENERATED_BODY()

public:
	ACapInteractionActor();

private:
	FTimerHandle EnterTimerHandle;

protected:
	UPROPERTY(EditAnywhere)
	float LongInteractionThreshold;
	
	virtual void BeginPlay() override;

public:
	virtual void InteractionEnter() override;
	virtual void InteractionExit() override;
	virtual void InteractionLongEnter();
};
