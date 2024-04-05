// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LobbyPlayerLinearColorFactory.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONE_2024_20_API ULobbyPlayerLinearColorFactory : public UObject
{
	GENERATED_BODY()
public:
	static FLinearColor GetLinearColor(int NumPlayers);
};
