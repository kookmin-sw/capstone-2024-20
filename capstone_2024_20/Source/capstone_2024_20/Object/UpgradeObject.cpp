#include "UpgradeObject.h"
#include "capstone_2024_20/MyIngameHUD.h"

AUpgradeObject::AUpgradeObject()
{
	CanBeOperated = true;
}

void AUpgradeObject::BeginPlay()
{
	Super::BeginPlay();
	MyInGameHUD = Cast<AMyIngameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

void AUpgradeObject::Operate()
{
	Super::Operate();
	MyInGameHUD->SetPopupUpgradeVisibility(true);
}
