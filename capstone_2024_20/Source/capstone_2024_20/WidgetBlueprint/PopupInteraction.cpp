#include "PopupInteraction.h"
#include "Components/TextBlock.h"

void UPopupInteraction::SetInteractionText(const FString& Text) const
{
	InteractionText->SetText(FText::FromString(Text));
}
