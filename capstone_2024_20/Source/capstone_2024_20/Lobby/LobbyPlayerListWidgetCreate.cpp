#include "LobbyPlayerListWidgetCreate.h"
#include "ULobbyPlayerListWidgetModifier.h"
#include "capstone_2024_20/01_Network/PlayerListWidgetModifier.h"

LobbyPlayerListWidgetCreate::LobbyPlayerListWidgetCreate(UWorld* World,
                                                         TSubclassOf<UPlayerListWidget> NewPlayerListWidgetClass, UPlayerListWidget** PlayerListWidget,
                                                         IPlayerList** PlayerList): PlayerListWidgetCreate(World, NewPlayerListWidgetClass, PlayerListWidget, PlayerList)
{
	
}

void LobbyPlayerListWidgetCreate::PostLogin(TArray<APlayerState*> PlayerStates)
{
	UPlayerListWidget* PlayerListWidget = CreatePlayerListWidget();
	GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Emerald,
									 TEXT("Create PostLogin"));
	*ControllerPlayerList = new ULobbyPlayerListWidgetModifier(PlayerListWidget);
	(**ControllerPlayerList).PostLogin(PlayerStates);
	*ControllerPlayerListWidget = PlayerListWidget;
}
