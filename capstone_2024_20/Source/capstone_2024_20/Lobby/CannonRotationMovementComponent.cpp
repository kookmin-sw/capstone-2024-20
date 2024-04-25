// Fill out your copyright notice in the Description page of Project Settings.


#include "CannonRotationMoveMentComponent.h"
#include "Net/UnrealNetwork.h"


void UCannonRotationMovementComponent::On_MovementBase()
{
	if (GetOwner()->GetLocalRole() != ROLE_SimulatedProxy)
		return;

	UpdatedComponent->SetRelativeRotation(MovementBase);
}

void UCannonRotationMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UCannonRotationMovementComponent, MovementBase, COND_SimulatedOnly);
}

UCannonRotationMovementComponent::UCannonRotationMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);
}

void UCannonRotationMovementComponent::SetUpdatedComponent(USceneComponent* NewUpdatedComponent)
{
	UpdatedComponent = IsValid(NewUpdatedComponent) ? NewUpdatedComponent : NULL;
}

void UCannonRotationMovementComponent::AddMovementRotation(FRotator WorldDirection)
{
	Velocity.Yaw = FMath::Clamp(WorldDirection.Yaw, -1.0f, 1.0f) * RotationSpeed;
}

void UCannonRotationMovementComponent::ControlledCharacterMove()
{
	const FRotator Rotator = UpdatedComponent->GetRelativeRotation();
	UpdatedComponent->SetRelativeRotation(Rotator + Velocity);
	Velocity = FRotator::ZeroRotator;
}

void UCannonRotationMovementComponent::ServerToMove_Implementation(FRotator Rotation)
{
	UpdatedComponent->SetRelativeRotation(Rotation);
	MovementBase = UpdatedComponent->GetRelativeRotation();
}


void UCannonRotationMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCannonRotationMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if(GetOwner()->GetLocalRole() >= ROLE_AutonomousProxy)
	{
		ControlledCharacterMove();

		const FRotator NewMoveRotation = UpdatedComponent->GetRelativeRotation();
		ServerToMove(NewMoveRotation);
	}
}
