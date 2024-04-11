// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class CAPSTONE_2024_20_API IPlayerList
{
public:
	virtual ~IPlayerList() = default;
	
	virtual void PostLogin(TArray<APlayerState*> PlayerStates) = 0;
	virtual void Logout(APlayerState* Exiting){}
};
