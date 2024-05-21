// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ReadyCharacterWidget.h"
#include "capstone_2024_20/01_Network/PlayerListController.h"
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
	virtual void Logout(AController* Exiting) override;
	virtual bool AllowCheats(APlayerController* P) override;
public:
	UPROPERTY(EditAnywhere)
	APlayerListController* PlayerListController;
	
	void GameStart();
	bool IsReadyAllPlayer() const;

	UFUNCTION()
	void SpawnPlayer(AController* NewPlayer);

	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	class UInGameRoomInfoWidget* RoomInfoWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInGameRoomInfoWidget> RoomInfoWidgetFactory;

	UFUNCTION()
	void PrintRoomCode();
};
