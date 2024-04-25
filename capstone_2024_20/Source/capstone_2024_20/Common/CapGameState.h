// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CapGameMode.h"
#include "GameFramework/GameState.h"
#include "CapGameState.generated.h"

UCLASS()
class CAPSTONE_2024_20_API ACapGameState : public AGameStateBase
{
	GENERATED_BODY()

	ACapGameState();

private:
	uint32 NumReadyPlayers : 0;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_RoomState, BlueprintReadOnly, VisibleInstanceOnly)
	FName RoomState;

	virtual void HandleNone();

	virtual void HandleJoiningUser();

	virtual void HandleAllReady();

	virtual void HandleGameStart();

public:
	virtual void OnRep_ReplicatedHasBegunPlay() override;
	virtual void HandleBeginPlay() override;
	virtual void HandleReady();

	UFUNCTION(Server)
	void ServerRPC_Ready();

	UFUNCTION()
	virtual void ServerRPC_Ready_Implementation();

	UFUNCTION()
	void OnRep_RoomState();

	FORCEINLINE FName GetRoomState() const { return RoomState; }

	UFUNCTION()
	void SetRoomState(const FName NewRoomState);
};
