#include "PopupGameOver.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UPopupGameOver::Initialize(const bool bHasAuthority) const
{
	if (bHasAuthority) // Server
	{
		TextBlockWaitingServer->SetVisibility(ESlateVisibility::Hidden);
		Button_0->SetVisibility(ESlateVisibility::Visible);
	}
	else // Client
	{
		TextBlockWaitingServer->SetVisibility(ESlateVisibility::Visible);
		Button_0->SetVisibility(ESlateVisibility::Hidden);
	}
}
