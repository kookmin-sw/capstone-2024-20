#include "MyIngameHUD.h"
#include "Components/ProgressBar.h"
#include "Upgrade/UpgradeWidgetElement.h"
#include "WidgetBlueprint/PopupDead.h"
#include "WidgetBlueprint/PopupCaution.h"

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
	
	PopupCautionClass = LoadClass<UPopupCaution>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/WidgetBlueprints/Sailing/BP_PopupCaution.BP_PopupCaution_C'"));
	PopupCaution = CreateWidget<UPopupCaution>(GetWorld(), PopupCautionClass);
	PopupCaution->AddToViewport();
	PopupCaution->SetVisibility(ESlateVisibility::Hidden);
	
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

void AMyIngameHUD::ShowPopupCaution(const FText& Text) const
{
	PopupCaution->SetCautionText(Text);
	PopupCaution->SetVisibility(ESlateVisibility::Visible);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		PopupCaution->SetVisibility(ESlateVisibility::Hidden);
	}, 3.0f, false);
}
