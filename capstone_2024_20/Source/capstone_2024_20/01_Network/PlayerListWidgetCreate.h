// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IPlayerList.h"
#include "UObject/Object.h"

class UPlayerListWidget;

class CAPSTONE_2024_20_API PlayerListWidgetCreate :public IPlayerList
{
public:
	PlayerListWidgetCreate(UWorld* World, TSubclassOf<UPlayerListWidget> NewPlayerListWidgetClass, UPlayerListWidget** PlayerListWidget, IPlayerList ** PlayerList);

	UWorld* CurrentWorld;
	TSubclassOf<UPlayerListWidget> PlayerListWidgetClass;
	UPlayerListWidget** ControllerPlayerListWidget;
	IPlayerList** ControllerPlayerList;
	
	virtual void PostLogin(TArray<APlayerState*> PlayerStates) override;

protected:
	UPlayerListWidget* CreatePlayerListWidget();
};
