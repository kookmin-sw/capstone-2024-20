// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LobbyPlayerState.h"
#include "ULobbyPlayerListWidgetModifier.h"
#include "capstone_2024_20/01_Network/PlayerListController.h"
#include "LobbyPlayerListController.generated.h"

UCLASS()
class CAPSTONE_2024_20_API ALobbyPlayerListController : public APlayerListController
{
	GENERATED_BODY()

public:
	ALobbyPlayerListController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Init() override;

private:
	ULobbyPlayerListWidgetModifier* LobbyPlayerListWidgetModifier;

public:
	UFUNCTION(NetMulticast, Reliable)
	void Refresh();

	UFUNCTION()
	void Refresh2();
	
	static void RegisterReadyEventTimer(UWorld* World, APlayerListController** LobbyPlayerListController,
	                               ALobbyPlayerState* PlayerState);

	static void RegisterReadyEventCallback(APlayerListController** LobbyPlayerListController,
								   ALobbyPlayerState* PlayerState);
};

