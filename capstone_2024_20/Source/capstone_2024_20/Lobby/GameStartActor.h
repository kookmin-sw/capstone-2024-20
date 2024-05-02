// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CapInteractionActor.h"
#include "GameStartActor.generated.h"

UCLASS()
class CAPSTONE_2024_20_API AGameStartActor : public ACapInteractionActor
{
	GENERATED_BODY()

public:
	AGameStartActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void InteractionEnter() override;
	virtual void InteractionLongEnter() override;
	virtual void InteractionExit() override;
};
