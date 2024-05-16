#include "MyIngameHUD.h"
#include "Upgrade/UpgradeWidgetElement.h"
#include "WidgetBlueprint/PopupDead.h"

void AMyIngameHUD::BeginPlay()
{
	Super::BeginPlay();

	InGameWidget = CreateWidget<UUserWidget>(GetWorld(), IngameWidgetClass);
	InGameWidget->AddToViewport();

	PopupDead = CreateWidget<UPopupDead>(GetWorld(), PopupDeadClass);
	
	PopupUpgrade = CreateWidget<UUpgradeWidget>(GetWorld(), PopupUpgradeClass);
	PopupUpgrade->AddToViewport();
	SetPopupUpgradeVisibility(false);
}

void AMyIngameHUD::SetPopupDeadVisibility(const bool bIsVisible) const
{
	if (bIsVisible)
	{
		PopupDead->AddToViewport();
	}
	else
	{
		PopupDead->RemoveFromParent();
	}
}

void AMyIngameHUD::SetPopupDeadTextByReviveCooldown(const float ReviveCooldown) const
{
	PopupDead->SetTextByReviveCooldown(ReviveCooldown);
}

UUpgradeWidget* AMyIngameHUD::GetPopupUpgrade() const
{
	return PopupUpgrade;
}

void AMyIngameHUD::SetPopupUpgradeVisibility(const bool bIsVisible) const
{
	PopupUpgrade->SetVisibilityWithBool(bIsVisible);
}
