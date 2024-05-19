#include "PopupEnemy.h"
#include "Components/ProgressBar.h"

void UPopupEnemy::SetHPProgressBarPercent(const float Percent) const
{
	HPProgressBar->SetPercent(Percent);
}