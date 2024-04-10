// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LobbyPlayerState.h"
#include "capstone_2024_20/MyCharacter.h"
#include "LobbyCharacter.generated.h"

UCLASS()
class CAPSTONE_2024_20_API ALobbyCharacter : public AMyCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALobbyCharacter();

private:
	ALobbyPlayerState* LobbyPlayerState;

	void Init();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	class ULobbyPlateWidgetComponent* WidgetComponent;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void SetReady();
};
