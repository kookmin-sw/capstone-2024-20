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

protected:
	virtual void RefreshItem(TArray<APlayerState*> PlayerStates, APlayerState* LogoutPlayerState = nullptr);

	UPlayerListWidget* PlayerListWidget;

	TMap<int32, int32> IdIndexMap;

public:
	void Refresh(TArray<APlayerState*> PlayerStates, APlayerState* LogoutPlayerState = nullptr);
	virtual void PostLogin(TArray<APlayerState*> PlayerStates) override;
	virtual void Logout(TArray<APlayerState*> PlayerStates, APlayerState* LogoutPlayerState) override;;
};
