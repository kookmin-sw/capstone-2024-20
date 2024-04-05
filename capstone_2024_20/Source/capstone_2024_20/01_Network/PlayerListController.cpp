// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerListController.h"

#include "PlayerListWidgetCreate.h"
#include "PlayerListWidgetModifier.h"


void UPlayerListController::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Purple,
		TEXT("Begin PPPPP"));
	PlayerListWidgetCreate* PlayerListWigetCreate = new PlayerListWidgetCreate(GetWorld(), PlayerListWigetClass,
	&PlayerListWidget, &PlayerListUpdate);
	PlayerListUpdate = PlayerListWigetCreate;
}

void UPlayerListController::PostLogin(APlayerController* NewPlayer)
{
	if(PlayerListUpdate)
	{
		GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Emerald,
			TEXT("NOT NOT"));
		PlayerListUpdate->PostLogin(NewPlayer);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Emerald,
		TEXT("nullptr"));
		GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Emerald,
			TEXT("nullptr2"));
	}
}

void UPlayerListController::Logout(AController* Exiting)
{
	IPlayerList::Logout(Exiting);
	PlayerListUpdate->Logout(Exiting);
}
