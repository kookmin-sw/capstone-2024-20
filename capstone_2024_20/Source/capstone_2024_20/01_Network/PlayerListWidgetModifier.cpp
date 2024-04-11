// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerListWidgetModifier.h"

#include "GameFramework/PlayerState.h"

UPlayerListWidgetModifier::UPlayerListWidgetModifier(UPlayerListWidget* NewPlayerListWidget)
{
	PlayerListWidget = NewPlayerListWidget;
}

void UPlayerListWidgetModifier::PostLogin(APlayerState* NewPlayer)
{
	GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Purple,
		TEXT("Modifier"));
	if (NewPlayer)
	{
		int32 Id = NewPlayer->GetPlayerId();
		FString PlayerName = NewPlayer->GetPlayerName();
		
		IdIndexMap.Add(Id, PlayerListWidget->GetLength());
		PlayerListWidget->Add(PlayerName);
	}
}

void UPlayerListWidgetModifier::Logout(APlayerState* Exiting)
{
	int32 Id = Exiting->GetPlayerId();
	int32* Index = IdIndexMap.Find(Id);

	PlayerListWidget->Remove(*Index);
}
