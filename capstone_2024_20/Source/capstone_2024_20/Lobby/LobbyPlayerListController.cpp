// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerListController.h"

#include "LobbyPlayerListWidgetCreate.h"
#include "LobbyPlayerState.h"
#include "GameFramework/GameStateBase.h"


class PlayerListWidgetCreate;

ALobbyPlayerListController::ALobbyPlayerListController(): APlayerListController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALobbyPlayerListController::BeginPlay()
{
	Super::BeginPlay();
}

void ALobbyPlayerListController::Init()
{
	PlayerListWidgetCreate* PlayerListWigetCreate = new LobbyPlayerListWidgetCreate(GetWorld(), PlayerListWigetClass,
		&PlayerListWidget, &PlayerListUpdate);

	PlayerListUpdate = PlayerListWigetCreate;
}

void ALobbyPlayerListController::Refresh2()
{
	GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Cyan,
	                                 TEXT("Refresh44"));

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			this,
			&ThisClass::Refresh,
			2.0f,
			false);
}

void ALobbyPlayerListController::RegisterReadyEventTimer(UWorld* World,
                                                         APlayerListController** LobbyPlayerListController,
                                                         ALobbyPlayerState* PlayerState)
{
	FTimerHandle TimerHandle;

	const FTimerDelegate TimerDelegate = FTimerDelegate::CreateLambda(
		&ALobbyPlayerListController::RegisterReadyEventCallback, LobbyPlayerListController, PlayerState);

	World->GetTimerManager().SetTimer(
		TimerHandle,
		TimerDelegate,
		4.0f,
		false);
}

void ALobbyPlayerListController::RegisterReadyEventCallback(APlayerListController** LobbyPlayerListController,
                                                            ALobbyPlayerState* PlayerState)
{
	if (PlayerState == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Cyan,
		                                 TEXT("Refresh11"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Cyan,
		                                 TEXT("Refresh22"));
		ALobbyPlayerListController* PlayerListController = Cast<ALobbyPlayerListController>(*LobbyPlayerListController);
		PlayerState->OnIsReadyChanged.BindDynamic(PlayerListController, &ALobbyPlayerListController::Refresh2);
	}
}

void ALobbyPlayerListController::Refresh_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Cyan,
	                                 TEXT("Refresh"));
	TArray<TObjectPtr<APlayerState>> PlayerStates = GetWorld()->GetGameState()->PlayerArray;

	if (LobbyPlayerListWidgetModifier == nullptr)
	{
		LobbyPlayerListWidgetModifier = new ULobbyPlayerListWidgetModifier(PlayerListWidget);
	}
	LobbyPlayerListWidgetModifier->Refresh(PlayerStates);
}
