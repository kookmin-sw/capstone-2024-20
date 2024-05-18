// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IPlayerList.h"
#include "PlayerListController.generated.h"

class UPlayerListWidget;

UCLASS()
class CAPSTONE_2024_20_API APlayerListController : public AActor
{
	GENERATED_BODY()

protected:
	APlayerListController();

public:
	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void Init();

protected:
	IPlayerList* PlayerListUpdate;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UPlayerListWidget> PlayerListWigetClass;

	UPROPERTY(EditAnywhere)
	UPlayerListWidget* PlayerListWidget;

public:
	UFUNCTION(NetMulticast, Reliable)
	void MutliRPC_PostLogin();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_Logout(APlayerState* ExitingPlayerState);
	
	UFUNCTION()
	virtual void PostLogin();
	UFUNCTION()
	virtual void Logout(AController* Exiting);

	static APlayerListController* Find(UWorld* World);
	static void PostLoginTimer(UWorld* World, APlayerListController** PlayerListController);
	static void PostLoginTimerCallback(APlayerListController** PlayerListController);
};
