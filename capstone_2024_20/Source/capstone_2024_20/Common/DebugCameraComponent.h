// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CameraPawn.h"
#include "Components/ActorComponent.h"
#include "DebugCameraComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CAPSTONE_2024_20_API UDebugCameraComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDebugCameraComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	ACameraPawn* Camera;
	APawn* PreCharacter;
public:
	UFUNCTION()
	void StartDebuging();
	UFUNCTION()
	void StopDebuging();
};
