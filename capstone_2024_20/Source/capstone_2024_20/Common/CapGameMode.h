// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CapGameMode.generated.h"

namespace FRoomState
{
	const FName None = FName(TEXT("None"));
	const FName JoiningUser = FName(TEXT("JoiningUser"));
	const FName AllReady = FName(TEXT("AllReady"));
	const FName GameStart = FName(TEXT("GameStart"));
}
UCLASS()
class CAPSTONE_2024_20_API ACapGameMode : public AGameModeBase
{
	GENERATED_BODY()

	ACapGameMode();
	
	virtual void PostLogin(APlayerController* NewPlayer) override;

public:
	int32 numnum = 0;
};
