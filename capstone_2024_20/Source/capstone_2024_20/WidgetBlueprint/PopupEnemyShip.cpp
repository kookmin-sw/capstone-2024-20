#include "PopupEnemyShip.h"
#include "Components/ProgressBar.h"

void UPopupEnemyShip::SetEnemyShipHPProgressBarPercent(const float Percent) const
{
	EnemyShipHPProgressBar->SetPercent(Percent);
}
