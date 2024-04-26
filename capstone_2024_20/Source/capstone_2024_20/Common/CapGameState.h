// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CapGameMode.h"
#include "GameFramework/GameState.h"
#include "CapGameState.generated.h"

DECLARE_MULTICAST_DELEGATE(FRoomStateChangedDelegate);

UCLASS()
class CAPSTONE_2024_20_API ACapGameState : public AGameStateBase
{
	GENERATED_BODY()

	ACapGameState();

private:
	uint32 NumReadyPlayers;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_RoomState, BlueprintReadOnly, VisibleInstanceOnly)
	FName RoomState;

	virtual void HandleNone();

	virtual void HandleJoiningUser();

	virtual void HandleAllReady();

	virtual void HandleGameStart();

public:
	FRoomStateChangedDelegate NoneDelegate;
	FRoomStateChangedDelegate JoiningUserDelegate;
	FRoomStateChangedDelegate AllReadyDelegate;
	FRoomStateChangedDelegate GameStartDelegate;
	FRoomStateChangedDelegate LocalReadyDelegate;
	
	virtual void OnRep_ReplicatedHasBegunPlay() override;
	virtual void HandleBeginPlay() override;
	virtual void HandleReady();

	virtual void Ready();

	UFUNCTION()
	void OnRep_RoomState();

	FORCEINLINE FName GetRoomState() const { return RoomState; }

	UFUNCTION()
	void SetRoomState(const FName NewRoomState);
};
