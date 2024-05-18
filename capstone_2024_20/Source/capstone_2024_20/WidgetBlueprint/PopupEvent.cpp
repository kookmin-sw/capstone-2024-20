#include "PopupEvent.h"
#include "Components/ProgressBar.h"

void UPopupEvent::SetDamageCoolDownProgressBarPercent(const float Percent) const
{
	ProgressBar->SetPercent(Percent);
}
