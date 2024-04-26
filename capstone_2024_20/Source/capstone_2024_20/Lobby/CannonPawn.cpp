// Fill out your copyright notice in the Description page of Project Settings.


#include "CannonPawn.h"

#include "CannonRotationMoveMentComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/ArrowComponent.h"
#include "Math/UnitConversion.h"


ACannonPawn::ACannonPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	M_ShooterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShooterMesh"));
	M_ShooterMesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSPawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(M_ShooterMesh);

	RotationMovementComponent = CreateDefaultSubobject<UCannonRotationMovementComponent>(
		TEXT("RotationMovementComponent"));
	RotationMovementComponent->SetUpdatedComponent(M_ShooterMesh);
}

void ACannonPawn::BeginPlay()
{
	Super::BeginPlay();
}

void ACannonPawn::Move(const FInputActionValue& Value)
{
	const FVector2D MoveVec = Value.Get<FVector2D>();
	
	FRotator MoveRotator = FRotator::ZeroRotator;
	MoveRotator.Yaw = MoveVec.X;
	
	RotationMovementComponent->AddMovementRotation(MoveRotator);
}

void ACannonPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACannonPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	}
}
