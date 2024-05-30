// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LobbySystem.generated.h"

class ULobbyWidget;

UCLASS()
class CAPSTONE_2024_20_API ALobbySystem : public AActor
{
	GENERATED_BODY()

public:
	ALobbySystem();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> LobbyWidgetClass;

	UPROPERTY(EditAnywhere)
	UUserWidget* SceneLobbyWidget;
};
