// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "capstone_2024_20/01_Network/PlayerListWidgetCreate.h"
#include "UObject/Object.h"

class UPlayerListWidget;

class CAPSTONE_2024_20_API LobbyPlayerListWidgetCreate :public PlayerListWidgetCreate
{
public:
	LobbyPlayerListWidgetCreate(UWorld* World, TSubclassOf<UPlayerListWidget> NewPlayerListWidgetClass, UPlayerListWidget** PlayerListWidget, IPlayerList ** PlayerList);
	
	virtual void PostLogin(TArray<APlayerState*> PlayerStates) override;
};
