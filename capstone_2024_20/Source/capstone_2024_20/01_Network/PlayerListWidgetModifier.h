// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IPlayerList.h"
#include "PlayerListWidget.h"
#include "UObject/Object.h"

/**
 * 
 */
class CAPSTONE_2024_20_API UPlayerListWidgetModifier : public IPlayerList
{
public:
	UPlayerListWidgetModifier(UPlayerListWidget* NewPlayerListWidget);
private:
	UPlayerListWidget* PlayerListWidget;

	TMap<int32, int32> IdIndexMap;

	void Refresh(TArray<APlayerState*> PlayerStates);
public:
	virtual void PostLogin(TArray<APlayerState*> PlayerStates) override;
	virtual void Logout(APlayerState* Exiting) override;
};
