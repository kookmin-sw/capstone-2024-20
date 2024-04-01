// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IPlayerList.h"
#include "PlayerListController.generated.h"

class UPlayerListWidget;

UCLASS()
class CAPSTONE_2024_20_API UPlayerListController :public UActorComponent, public IPlayerList
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UPlayerListWidget> PlayerListWigetClass;
	
	UPlayerListWidget* PlayerListWidget;
	IPlayerList* PlayerListUpdate;

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
};
