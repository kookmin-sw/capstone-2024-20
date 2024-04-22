// Fill out your copyright notice in the Description page of Project Settings.


#include "CannonPawn.h"

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
}

void ACannonPawn::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ACannonPawn::Move(const FInputActionValue& Value)
{
	const FVector2D MoveVec = Value.Get<FVector2D>();

	const FRotator CurrentRotation = M_ShooterMesh->GetRelativeRotation();
	float NewYaw = CurrentRotation.Yaw + (MoveVec.X * RotationSpeed);

	FRotator NewRotation = FRotator(CurrentRotation.Pitch, NewYaw, CurrentRotation.Roll);
	M_ShooterMesh->SetRelativeRotation(NewRotation);

	Velocity.Yaw = FMath::Clamp(NewYaw, -1.0f, 1.0f) * RotationSpeed;
}

void ACannonPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FRotator Rotator = M_ShooterMesh->GetRelativeRotation();
	M_ShooterMesh->SetRelativeRotation(Rotator + (Velocity * DeltaTime));
	Velocity = FRotator::ZeroRotator;
}

void ACannonPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	}
}
