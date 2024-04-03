// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LobbyPlayerState.h"
#include "ReadyCharacterWidget.h"
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

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UWidgetComponent* WidgetComponent;
public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditAnywhere)
	UReadyCharacterWidget* ReadyCharacterWidget;

	UFUNCTION()
	void SetReady();
};
