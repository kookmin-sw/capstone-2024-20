#include "MyIngameHUD.h"
#include "Components/ProgressBar.h"
#include "Upgrade/UpgradeWidgetElement.h"
#include "WidgetBlueprint/PopupDead.h"

void AMyIngameHUD::BeginPlay()
{
	if (bIsCalledBeginPlay)
	{
		return;
	}
	
	Super::BeginPlay();

	InGameWidget = CreateWidget<UUserWidget>(GetWorld(), IngameWidgetClass);
	InGameWidget->AddToViewport();

	PopupDead = CreateWidget<UPopupDead>(GetWorld(), PopupDeadClass);
	
	PopupUpgrade = CreateWidget<UUpgradeWidget>(GetWorld(), PopupUpgradeClass);
	PopupUpgrade->AddToViewport();
	SetPopupUpgradeVisibility(false);

	EnemyShipProgressBar = Cast<UProgressBar>(InGameWidget->GetWidgetFromName(TEXT("EnemyShipHPProgressBar")));
	EnemyShipProgressBar->SetVisibility(ESlateVisibility::Hidden);
	
	bIsCalledBeginPlay = true;
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

bool AMyIngameHUD::GetEnemyShipHPProgressBarVisibility() const
{
	return EnemyShipProgressBar->IsVisible();
}

void AMyIngameHUD::SetEnemyShipHPProgressBarVisibility(const bool bIsVisible) const
{
	EnemyShipProgressBar->SetVisibility(bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void AMyIngameHUD::SetEnemyShipHPProgressBarPercent(const float Percent) const
{
	EnemyShipProgressBar->SetPercent(Percent);
}
