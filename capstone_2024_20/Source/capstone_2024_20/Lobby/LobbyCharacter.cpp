// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyCharacter.h"

#include "LobbyGameMode.h"
#include "LobbyPlayerLinearColorFactory.h"
#include "LobbyPlateWidgetComponent.h"
#include "Components/WidgetComponent.h"

ALobbyCharacter::ALobbyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	WidgetComponent = CreateDefaultSubobject<ULobbyPlateWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(GetRootComponent());
	NamePlateWidget->DestroyComponent();
}

void ALobbyCharacter::Init()
{
	LobbyPlayerState = Cast<ALobbyPlayerState>(GetPlayerState());
	LobbyPlayerState->OnIsReadyChanged.BindDynamic(this, &ALobbyCharacter::SetReady);

	const int PlayerNumber = LobbyPlayerState->PlayerNumber;
	if (PlayerNumber != 1)
	{
		const FLinearColor PlayerColor = ULobbyPlayerLinearColorFactory::GetLinearColor(PlayerNumber);

		WidgetComponent->ChangeColor(PlayerColor);
		WidgetComponent->SetVisibilityFromBool(false);
	}
	else
	{
		FString Path = TEXT("/Game/WidgetBlueprints/Lobby/BP_RoomManagerWidget.BP_RoomManagerWidget_C");
		WidgetComponent->ChangeWidget(Path);
	}

	if(IsLocallyControlled())
	{
		const FString PlayerName = LobbyPlayerState->GetPlayerName();
		WidgetComponent->SetName(PlayerName);
	}
	
	if (IsLocallyControlled())
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
	WidgetComponent->SetVisibilityFromBool(LobbyPlayerState->IsReady());
}
