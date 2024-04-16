// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerListWidgetModifier.h"

#include "GameFramework/PlayerState.h"

UPlayerListWidgetModifier::UPlayerListWidgetModifier(UPlayerListWidget* NewPlayerListWidget)
{
	PlayerListWidget = NewPlayerListWidget;
}

void UPlayerListWidgetModifier::Refresh(TArray<APlayerState*> PlayerStates, APlayerState* LogoutPlayerState)
{
	PlayerListWidget->Clear();
	for (int i = 0; i < PlayerStates.Num(); i++)
	{
		if (LogoutPlayerState == PlayerStates[i])
			continue;

		FString PlayerName = PlayerStates[i]->GetPlayerName();
		PlayerListWidget->Add(PlayerName);
	}
}

void UPlayerListWidgetModifier::PostLogin(TArray<APlayerState*> PlayerStates)
{
	Refresh(PlayerStates);
}

void UPlayerListWidgetModifier::Logout(TArray<APlayerState*> PlayerStates, APlayerState* LogoutPlayerState)
{
	Refresh(PlayerStates, LogoutPlayerState);
}
