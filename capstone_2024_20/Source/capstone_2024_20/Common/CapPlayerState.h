// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CapPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONE_2024_20_API ACapPlayerState : public APlayerState
{
	GENERATED_BODY()

	ACapPlayerState();

	virtual void BeginPlay() override;
public:
	UFUNCTION(Server, Reliable)
	void ServerRPC_StartReady();
};
