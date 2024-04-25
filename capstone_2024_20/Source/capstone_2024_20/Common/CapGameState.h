// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CapGameMode.h"
#include "GameFramework/GameState.h"
#include "CapGameState.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONE_2024_20_API ACapGameState : public AGameState
{
	GENERATED_BODY()

	ACapGameState();

protected:
	UPROPERTY(ReplicatedUsing=OnRep_RoomState, BlueprintReadOnly, VisibleInstanceOnly)
	FName RoomState;

	virtual void HandleNone();

	virtual void HandleJoiningUser();

	virtual void HandleAllReady();

	virtual void HandleGameStart();

public:
	UFUNCTION()
	void OnRep_RoomState();

	FORCEINLINE FName GetRoomState() const { return RoomState; }

	UFUNCTION()
	void SetRoomState(const FName NewRoomState);
};
