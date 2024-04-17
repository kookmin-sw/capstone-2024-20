// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugCameraComponent.h"

#include "CameraPawn.h"
#include "Camera/CameraActor.h"
#include "capstone_2024_20/MyCharacter.h"
#include "GameFramework/Character.h"


// Sets default values for this component's properties
UDebugCameraComponent::UDebugCameraComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDebugCameraComponent::BeginPlay()
{
	Super::BeginPlay();
	

}

void UDebugCameraComponent::StartDebuging()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PreCharacter = PlayerController->GetCharacter();

	const FVector Location = PreCharacter->GetActorLocation();
	const FRotator Rotation = PreCharacter->GetActorRotation();
	
	Camera = GetWorld()->SpawnActor<ACameraPawn>(ACameraPawn::StaticClass(), Location, Rotation);
	if (Camera)
	{
		PlayerController->Possess(Camera);
	}
}

void UDebugCameraComponent::StopDebuging()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->Possess(PreCharacter);
	GetWorld()->DestroyActor(Camera);
}

