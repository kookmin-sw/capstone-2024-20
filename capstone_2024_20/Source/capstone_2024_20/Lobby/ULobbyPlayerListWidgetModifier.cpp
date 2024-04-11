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
	
	for(int i=0;i<PlayerStates.Num();i++)
	{
		ALobbyPlayerState* PlayerState = Cast<ALobbyPlayerState>(PlayerStates[i]);
		ULobbyPlayerListElementWidget* LobbyPlayerListElement =
			Cast<ULobbyPlayerListElementWidget>(PlayerListWidget->PlayerElements[i]);

		if(PlayerState->PlayerNumber != 1)
		{
			GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Magenta,
				TEXT("Refresh PlayerState"));
			if(PlayerState->IsReady()== true)
			{
				GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Magenta,
				TEXT("Refresh True"));
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Magenta,
				TEXT("Refresh False"));
			}
			LobbyPlayerListElement->SetVisibilityFromBool(PlayerState->IsReady());
			FLinearColor LinearColor = ULobbyPlayerLinearColorFactory::GetLinearColor(i+1);
			LobbyPlayerListElement->ChangeColor(LinearColor);
		}
	}
}
