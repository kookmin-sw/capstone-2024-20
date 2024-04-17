// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "capstone_2024_20/01_Network/PlayerListWidgetModifier.h"
#include "UObject/Object.h"

/**
 * 
 */
class CAPSTONE_2024_20_API ULobbyPlayerListWidgetModifier : public UPlayerListWidgetModifier
{
public:
	ULobbyPlayerListWidgetModifier(UPlayerListWidget* NewPlayerListWidget);

protected:
	virtual void RefreshItem(TArray<APlayerState*> PlayerStates, APlayerState* LogoutPlayerState) override;
};


