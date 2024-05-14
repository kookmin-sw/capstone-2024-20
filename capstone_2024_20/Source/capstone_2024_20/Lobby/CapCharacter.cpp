// Fill out your copyright notice in the Description page of Project Settings.


#include "CapCharacter.h"

#include "CapInteractionActor.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedPlayerInput.h"
#include "Camera/CameraComponent.h"
#include "capstone_2024_20/CharacterChangerComponent.h"
#include "capstone_2024_20/MyAudioInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

ACapCharacter::ACapCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CharacterChangerComponent = CreateDefaultSubobject<UCharacterChangerComponent>(TEXT("CharacterChangerComponent"));
	CharacterChangerComponent->SetIsReplicated(true);
	InitMovement();
}

void ACapCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocallyControlled())
	{
		GetGameInstance<UMyAudioInstance>()->OnChangeCharacterTypeDelegate.AddUObject(
			CharacterChangerComponent, &UCharacterChangerComponent::Change);
	}
}

void ACapCharacter::Move(const FInputActionValue& Value)
{
	if (bIsMovement == false)
		return;

	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller == nullptr)
		return;

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator Yaw(0, Rotation.Yaw, 0);

	const FVector RightVector = FRotationMatrix(Yaw).GetUnitAxis(EAxis::Y);
	const FVector ForwardVector = FRotationMatrix(Yaw).GetUnitAxis(EAxis::X);

	AddMovementInput(RightVector, MovementVector.X);
	AddMovementInput(ForwardVector, MovementVector.Y);
}

void ACapCharacter::Interact()
{
	if (CapInteractionActor)
	{
		CapInteractionActor->InteractionEnter();
	}
}

void ACapCharacter::InteractCancel()
{
	if (CapInteractionActor)
	{
		CapInteractionActor->InteractionExit();
	}
}

void ACapCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACapCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::Interact);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &ThisClass::InteractCancel);
	}
}

void ACapCharacter::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
                              bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse,
                              const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	ACapInteractionActor* ActorOhter = Cast<ACapInteractionActor>(Other);
	if (ActorOhter)
	{
		CapInteractionActor = ActorOhter;
	}
}

void ACapCharacter::SetIsMovement(const bool bNewValue)
{
	bIsMovement = bNewValue;
}

void ACapCharacter::ServerRPC_SetLocationAndRotation_Implementation(FVector NewLocation, FRotator NewRotation)
{
	SetActorLocationAndRotation(NewLocation, NewRotation);
	MulticastRPC_SetLocationAndRotation(NewLocation, NewRotation);
}

void ACapCharacter::MulticastRPC_SetLocationAndRotation_Implementation(FVector NewLocation, FRotator NewRotation)
{
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 50000.0f, 0.0f);
	FVector Direction = NewRotation.Vector().GetSafeNormal();
	AddMovementInput(Direction, 1.0f);
	SetActorLocationAndRotation(NewLocation, NewRotation);
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
}

void ACapCharacter::InitMovement()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 800.f;
	GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	GetCharacterMovement()->MaxAcceleration = 1400.0f;
}
