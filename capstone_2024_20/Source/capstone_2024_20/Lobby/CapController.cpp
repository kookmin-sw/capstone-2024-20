// Fill out your copyright notice in the Description page of Project Settings.


#include "CapController.h"

#include "CapCharacter.h"
#include "EnhancedInputSubsystems.h"

void ACapController::BeginPlay()
{
	Super::BeginPlay();
}

void ACapController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	RefreshMappingContext(GetPawn());
}

void ACapController::OnRep_Pawn()
{
	Super::OnRep_Pawn();
	
	if(IsLocalPlayerController() == false)
		return;
		
	RefreshMappingContext(GetPawn());
}

void ACapController::RefreshMappingContext(APawn* InPawn) const
{
	ICapPawn* CapPawn = Cast<ICapPawn>(InPawn);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if(Subsystem == nullptr || CapPawn == nullptr)
		return;

	Subsystem->AddMappingContext(CapPawn->GetMappingContext(), 0);
}
