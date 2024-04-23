// Fill out your copyright notice in the Description page of Project Settings.


#include "CannonRotationMoveMentComponent.h"


UCannonRotationMovementComponent::UCannonRotationMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCannonRotationMovementComponent::SetUpdatedComponent(USceneComponent* NewUpdatedComponent)
{
	UpdatedComponent = IsValid(NewUpdatedComponent) ? NewUpdatedComponent : NULL;
}

void UCannonRotationMovementComponent::AddMovementRotation(FRotator WorldDirection)
{
	Velocity.Yaw = FMath::Clamp(WorldDirection.Yaw, -1.0f, 1.0f) * RotationSpeed;
}


void UCannonRotationMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCannonRotationMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	const FRotator Rotator = UpdatedComponent->GetRelativeRotation();
	UpdatedComponent->SetRelativeRotation(Rotator + Velocity);
	Velocity = FRotator::ZeroRotator;
}

