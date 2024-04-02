// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LobbyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONE_2024_20_API ALobbyPlayerState : public APlayerState
{
	GENERATED_BODY()

private:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(ReplicatedUsing=OnRep_IsReady)
	bool bIsReady = false;

	bool bIsCanReady = true;

	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void OnRep_IsReady();

	UFUNCTION(Server, Reliable)
	void Server_SetReady(bool IsReady);

	UFUNCTION()
	void TestReady();

	bool IsReady() const;
};
