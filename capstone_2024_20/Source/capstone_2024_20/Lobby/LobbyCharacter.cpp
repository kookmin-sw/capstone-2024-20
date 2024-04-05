// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyCharacter.h"

#include "LobbyGameMode.h"
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

	ReadyCharacterWidget = Cast<UReadyCharacterWidget>(
		WidgetComponent->GetWidget());

	const int PlayerNumber = LobbyPlayerState->PlayerNumber;
	if (PlayerNumber != 1)
	{
		const FLinearColor PlayerColor = ULobbyPlayerLinearColorFactory::GetLinearColor(PlayerNumber);

		ReadyCharacterWidget->ChangeColor(PlayerColor);
		ReadyCharacterWidget->SetVisibilityFromBool(false);
	}
	else
	{
		UClass* BlueprintClass = LoadClass<UReadyCharacterWidget>(nullptr,
		TEXT("/Game/WidgetBlueprints/Lobby/BP_RoomManagerWidget.BP_RoomManagerWidget_C"));

		GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Yellow,
			TEXT("Server"));
		if (BlueprintClass)
		{
			GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Yellow,
			TEXT("not null"));
			UUserWidget* NewWidget = CreateWidget<UUserWidget>(GetWorld(),
			                                                   BlueprintClass);
			SetWidget(NewWidget);
		}

		ReadyCharacterWidget = Cast<UReadyCharacterWidget>(
			WidgetComponent->GetWidget());
	}

	const FString PlayerName = LobbyPlayerState->GetPlayerName();
	ReadyCharacterWidget->SetName(PlayerName);

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
	ReadyCharacterWidget->SetVisibilityFromBool(LobbyPlayerState->IsReady());
}

void ALobbyCharacter::SetWidget(UUserWidget* Widget)
{
	WidgetComponent->SetWidget(Widget);
	ReadyCharacterWidget = Cast<UReadyCharacterWidget>(
		WidgetComponent->GetWidget());
}
