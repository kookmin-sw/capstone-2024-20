#include "PopupCaution.h"
#include "Components/TextBlock.h"

void UPopupCaution::SetCautionText(const FText& Text) const
{
	CautionText->SetText(Text);
}
