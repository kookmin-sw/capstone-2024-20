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
	class URoomPasswordInputPopupWidget* RoomPasswordInputPopupWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UJoinMenuWidget> MainWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<URoomPasswordInputPopupWidget> RoomPasswordInputPopupWidgetClass;
private:
	UFUNCTION()
	void OnClickRefreshButton();
	
	void RefreshRoomListUI(bool bWasSuccessful) const;

public:
	virtual bool AllowCheats(APlayerController* P) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;
};