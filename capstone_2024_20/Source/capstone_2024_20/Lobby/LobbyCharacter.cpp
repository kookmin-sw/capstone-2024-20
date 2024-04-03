// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyCharacter.h"

#include "LobbyPlayerLinearColorFactory.h"
#include "Components/WidgetComponent.h"

ALobbyCharacter::ALobbyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(GetRootComponent());
}

void ALobbyCharacter::Init()
{
	LobbyPlayerState = Cast<ALobbyPlayerState>(GetPlayerState());
	LobbyPlayerState->OnIsReadyChanged.BindDynamic(this, &ALobbyCharacter::SetReady);

	const int PlayerNumber = LobbyPlayerState->PlayerNumber;
	const FLinearColor PlayerColor = ULobbyPlayerLinearColorFactory::GetLinearColor(PlayerNumber);
	ReadyCharacterWidget->ChnageColor(PlayerColor);
	
	if(IsLocallyControlled())
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			PlayerController->InputComponent->BindKey(EKeys::C, IE_Pressed,
													  LobbyPlayerState, &ALobbyPlayerState::SetReady);
		}
	}
}

void ALobbyCharacter::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::Init, 2.0f, false);

	ReadyCharacterWidget = Cast<UReadyCharacterWidget>(
		WidgetComponent->GetWidget());

	ReadyCharacterWidget->SetVisibilityFromBool(false);
}

void ALobbyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALobbyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ALobbyCharacter::SetReady()
{
	ReadyCharacterWidget->SetVisibilityFromBool(LobbyPlayerState->IsReady());
}