// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CapGameMode.generated.h"

namespace RoomState
{
	const FName EnteringMap = FName(TEXT("EnteringMap"));
	const FName JoiningUser = FName(TEXT("JoiningUser"));
	const FName AllReady = FName(TEXT("AllReady"));
	const FName GameStart = FName(TEXT("GameStart"));
}
UCLASS()
class CAPSTONE_2024_20_API ACapGameMode : public AGameModeBase
{
	GENERATED_BODY()
};
