// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ReadyCharacterWidget.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONE_2024_20_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

	ALobbyGameMode();

	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
public:
	void GameStart();
	bool IsReadyAllPlayer() const;
};
