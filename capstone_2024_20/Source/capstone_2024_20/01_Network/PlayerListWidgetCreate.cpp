// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerListWidgetCreate.h"

#include "Blueprint/UserWidget.h"
#include "InGameRoomInfoWidget.h"
#include "PlayerListWidget.h"
#include "PlayerListWidgetModifier.h"

PlayerListWidgetCreate::PlayerListWidgetCreate(UWorld* World, TSubclassOf<UPlayerListWidget> NewPlayerListWidgetClass,
                                               UPlayerListWidget** PlayerListWidget, IPlayerList** PlayerList)
{
	CurrentWorld = World;
	PlayerListWidgetClass = NewPlayerListWidgetClass;
	ControllerPlayerListWidget = PlayerListWidget;
	ControllerPlayerList = PlayerList;
}

void PlayerListWidgetCreate::PostLogin(TArray<APlayerState*> PlayerStates)
{
	UPlayerListWidget* PlayerListWidget = CreatePlayerListWidget();
	GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Emerald,
	                                 TEXT("Create PostLogin"));
	*ControllerPlayerList = new UPlayerListWidgetModifier(PlayerListWidget);
	(*ControllerPlayerList)->PostLogin(PlayerStates);
	*ControllerPlayerListWidget = PlayerListWidget;
}

UPlayerListWidget* PlayerListWidgetCreate::CreatePlayerListWidget()
{
	UPlayerListWidget* WidgetInstance = CreateWidget<UPlayerListWidget>(CurrentWorld, PlayerListWidgetClass);
	WidgetInstance->AddToViewport(100);

	return WidgetInstance;
}
