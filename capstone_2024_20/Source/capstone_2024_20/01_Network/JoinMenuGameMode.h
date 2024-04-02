// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "JoinMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONE_2024_20_API AJoinMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	class UJoinMenuWidget* MainUI;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UJoinMenuWidget> MainWidget;
private:
	UFUNCTION()
	void OnClickRefreshButton();
	
	void RefreshRoomListUI(bool bWasSuccessful) const;
};