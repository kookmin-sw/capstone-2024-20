// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IPlayerList.h"
#include "PlayerListController.generated.h"

class UPlayerListWidget;

UCLASS()
class CAPSTONE_2024_20_API APlayerListController :public AActor, public IPlayerList
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
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void Logout(AController* Exiting) override;

	static APlayerListController* Create(UWorld* World);


	UFUNCTION(NetMulticast, Reliable)
	void TTEE();
};
