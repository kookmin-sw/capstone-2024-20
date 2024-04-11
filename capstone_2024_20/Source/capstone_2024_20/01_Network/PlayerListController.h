// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IPlayerList.h"
#include "PlayerListController.generated.h"

class UPlayerListWidget;

UCLASS()
class CAPSTONE_2024_20_API APlayerListController :public AActor
{
	GENERATED_BODY()

	APlayerListController();
public:
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UPlayerListWidget> PlayerListWigetClass;
	
	UPlayerListWidget* PlayerListWidget;
	IPlayerList* PlayerListUpdate;

public:
	UFUNCTION(NetMulticast, Reliable)
	void MutliRPC_PostLogin();

	UFUNCTION()
	void PostLogin();
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void Logout(APlayerState* Exiting);

	static APlayerListController* Find(UWorld* World);
	static void PostLoginTimer(UWorld* World, APlayerListController** PlayerListController, APlayerState* PlayerState);
	static void PostLoginTimerCallback(APlayerListController** PlayerListController, APlayerState* PlayerState);
};
