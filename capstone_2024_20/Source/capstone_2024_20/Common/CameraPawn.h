// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "CameraPawn.generated.h"

UCLASS()
class CAPSTONE_2024_20_API ACameraPawn : public APawn
{
	GENERATED_BODY()

public:
	ACameraPawn();

protected:
	virtual void BeginPlay() override;
	UCameraComponent* CameraComponent;
public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
