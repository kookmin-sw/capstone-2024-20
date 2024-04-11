// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerListWidgetModifier.h"

#include "GameFramework/PlayerState.h"

UPlayerListWidgetModifier::UPlayerListWidgetModifier(UPlayerListWidget* NewPlayerListWidget)
{
	PlayerListWidget = NewPlayerListWidget;
}

void UPlayerListWidgetModifier::Refresh(TArray<APlayerState*> PlayerStates)
{
	PlayerListWidget->Clear();
	for(int i=0;i<PlayerStates.Num();i++)
	{
		FString PlayerName = PlayerStates[i]->GetPlayerName();
		PlayerListWidget->Add(PlayerName);
	}
}

void UPlayerListWidgetModifier::PostLogin(TArray<APlayerState*> PlayerStates)
{
	GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Purple,
	                                 TEXT("Modifier"));
	Refresh(PlayerStates);
}

void UPlayerListWidgetModifier::Logout(APlayerState* Exiting)
{
	int32 Id = Exiting->GetPlayerId();
	int32* Index = IdIndexMap.Find(Id);

	PlayerListWidget->Remove(*Index);
}
