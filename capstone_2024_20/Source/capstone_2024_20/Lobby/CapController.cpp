// Fill out your copyright notice in the Description page of Project Settings.


#include "CapController.h"

#include "CapCharacter.h"
#include "EngineUtils.h"
#include "EnhancedInputComponent.h"
#include "MappingContextSwitcher.h"
#include "capstone_2024_20/Common/ChatWidget.h"
#include "GameFramework/HUD.h"

ACapController::ACapController()
{
	MappingContextSwitcher = CreateDefaultSubobject<UMappingContextSwitcher>(TEXT("MappingContextSwitcher"));
}

void ACapController::BeginPlay()
{
	Super::BeginPlay();
	ChatWidget = CreateWidget<UChatWidget>(GetWorld(), ChatWidgetClass);
	if(ChatWidget)
	{
		ChatWidget->AddToViewport();
	}
}

void ACapController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(EnterAction, ETriggerEvent::Started, this, &ThisClass::OnEnter);
	}
}

void ACapController::OnEnter()
{
	if(ChatWidget)
	{
		ChatWidget->EnableChat();
	}
}

void ACapController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	RefreshMappingContext(GetPawn());
}

void ACapController::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	if (IsLocalPlayerController() == false)
		return;

	RefreshMappingContext(GetPawn());
}

void ACapController::RefreshMappingContext(APawn* InPawn) const
{
	IMappingContextGetter* CapPawn = Cast<IMappingContextGetter>(InPawn);
	if (CapPawn == nullptr)
		return;

	MappingContextSwitcher->ReplaceMappingContext(CapPawn->GetMappingContext());
}
