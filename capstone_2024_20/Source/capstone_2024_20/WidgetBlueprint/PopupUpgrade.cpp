#include "PopupUpgrade.h"
#include "capstone_2024_20/Sailing/SailingSystem.h"
#include "Kismet/GameplayStatics.h"

void UPopupUpgrade::Upgrade() const
{
	// Todo@autumn - This is a temporary implementation, need to replace it
	AActor* SailingSystem = UGameplayStatics::GetActorOfClass(GetWorld(), ASailingSystem::StaticClass());
	if (SailingSystem == nullptr)
	{
		return;
	}

	const ASailingSystem* SailingSystemInstance = Cast<ASailingSystem>(SailingSystem);
	SailingSystemInstance->UpgradeMyShip();
}
