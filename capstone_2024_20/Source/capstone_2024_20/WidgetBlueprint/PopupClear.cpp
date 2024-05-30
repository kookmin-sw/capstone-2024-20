#include "PopupClear.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UPopupClear::Initialize(const bool bHasAuthority) const
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
