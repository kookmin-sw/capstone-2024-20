// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraPawn.h"

ACameraPawn::ACameraPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	RootComponent = CameraComponent;
}

void ACameraPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

