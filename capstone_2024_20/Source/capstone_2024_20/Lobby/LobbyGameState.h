// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "capstone_2024_20/Common/CapGameState.h"
#include "LobbyGameState.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONE_2024_20_API ALobbyGameState : public ACapGameState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Network")
	class ANetworkService* NetworkService;

	UFUNCTION()
	FName GetRoomCode() const;

	UFUNCTION()
	FName GetRoomName() const;
};
