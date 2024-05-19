#include "ULobbyPlayerListWidgetModifier.h"

#include "LobbyPlayerLinearColorFactory.h"
#include "LobbyPlayerListElementWidget.h"
#include "LobbyPlayerState.h"

ULobbyPlayerListWidgetModifier::ULobbyPlayerListWidgetModifier(UPlayerListWidget* NewPlayerListWidget):
	UPlayerListWidgetModifier(NewPlayerListWidget)
{
}

void ULobbyPlayerListWidgetModifier::RefreshItem(TArray<APlayerState*> PlayerStates, APlayerState* LogoutPlayerState)
{
	UPlayerListWidgetModifier::RefreshItem(PlayerStates, LogoutPlayerState);

	int32 Index = 0;
	for (int32 i = 0; i < PlayerStates.Num(); i++)
	{
		ULobbyPlayerListElementWidget* LobbyPlayerListElement =
			Cast<ULobbyPlayerListElementWidget>(PlayerListWidget->PlayerElements[Index]);

		ALobbyPlayerState* PlayerState = Cast<ALobbyPlayerState>(PlayerStates[i]);

		if (PlayerStates[i] == LogoutPlayerState)
			continue;
		if (PlayerState->PlayerNumber != 1)
		{
			LobbyPlayerListElement->SetVisibilityFromBool(PlayerState->IsReady());
			FLinearColor LinearColor = ULobbyPlayerLinearColorFactory::GetLinearColor(i + 1);
			LobbyPlayerListElement->ChangeColor(LinearColor);
		}
		else
		{
			LobbyPlayerListElement->SetVisibilityFromBool(true);
		}

		Index++;
	}
}
