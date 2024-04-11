#include "ULobbyPlayerListWidgetModifier.h"

#include "LobbyPlayerLinearColorFactory.h"
#include "LobbyPlayerListElementWidget.h"
#include "LobbyPlayerState.h"

ULobbyPlayerListWidgetModifier::ULobbyPlayerListWidgetModifier(UPlayerListWidget* NewPlayerListWidget):
	UPlayerListWidgetModifier(NewPlayerListWidget)
{
}

void ULobbyPlayerListWidgetModifier::Refresh(TArray<APlayerState*> PlayerStates)
{
	UPlayerListWidgetModifier::Refresh(PlayerStates);

	for (int i = 0; i < PlayerStates.Num(); i++)
	{
		ALobbyPlayerState* PlayerState = Cast<ALobbyPlayerState>(PlayerStates[i]);
		ULobbyPlayerListElementWidget* LobbyPlayerListElement =
			Cast<ULobbyPlayerListElementWidget>(PlayerListWidget->PlayerElements[i]);

		if (PlayerState->PlayerNumber != 1)
		{
			LobbyPlayerListElement->SetVisibilityFromBool(PlayerState->IsReady());
			FLinearColor LinearColor = ULobbyPlayerLinearColorFactory::GetLinearColor(i + 1);
			LobbyPlayerListElement->ChangeColor(LinearColor);
		}
	}
}
