#include "MyIngameHUD.h"
#include "Components/ProgressBar.h"
#include "Upgrade/UpgradeWidgetElement.h"
#include "WidgetBlueprint/PopupDead.h"
#include "WidgetBlueprint/PopupCaution.h"
#include "WidgetBlueprint/PopupEnemyShip.h"
#include "WidgetBlueprint/PopupGameOver.h"
#include "WidgetBlueprint/PopupClear.h"

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

	PopupEnemyShipClass = LoadClass<UPopupEnemyShip>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/WidgetBlueprints/EnemyShip/BP_PopupEnemyShip.BP_PopupEnemyShip_C'"));
	PopupEnemyShip = CreateWidget<UPopupEnemyShip>(GetWorld(), PopupEnemyShipClass);
	PopupEnemyShip->AddToViewport();
	PopupEnemyShip->SetVisibility(ESlateVisibility::Hidden);
	
	PopupCautionClass = LoadClass<UPopupCaution>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/WidgetBlueprints/Sailing/BP_PopupCaution.BP_PopupCaution_C'"));
	PopupCaution = CreateWidget<UPopupCaution>(GetWorld(), PopupCautionClass);
	PopupCaution->AddToViewport();
	PopupCaution->SetVisibility(ESlateVisibility::Hidden);

	PopupGameOverClass = LoadClass<UPopupGameOver>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/WidgetBlueprints/StageFailPopUpWidget.StageFailPopUpWidget_C'"));
	PopupGameOver = CreateWidget<UPopupGameOver>(GetWorld(), PopupGameOverClass);
	PopupGameOver->AddToViewport();
	PopupGameOver->SetVisibility(ESlateVisibility::Hidden);

	PopupClearClass = LoadClass<UPopupClear>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/WidgetBlueprints/StageClearPopUpWidget.StageClearPopUpWidget_C'"));
	PopupClear = CreateWidget<UPopupClear>(GetWorld(), PopupClearClass);
	PopupClear->AddToViewport();
	PopupClear->SetVisibility(ESlateVisibility::Hidden);
	
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

bool AMyIngameHUD::GetPopupEnemyShipVisibility() const
{
	return PopupEnemyShip->GetVisibility() == ESlateVisibility::Visible;
}

void AMyIngameHUD::SetPopupEnemyShipVisibility(const bool bIsVisible) const
{
	PopupEnemyShip->SetVisibility(bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void AMyIngameHUD::SetEnemyShipHPProgressBarPercent(const float Percent) const
{
	PopupEnemyShip->SetEnemyShipHPProgressBarPercent(Percent);
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

void AMyIngameHUD::ShowPopupGameOver(const bool bHasAuthority) const
{
	PopupGameOver->SetVisibility(ESlateVisibility::Visible);
	PopupGameOver->Initialize(bHasAuthority);

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->bShowMouseCursor = true;
}

void AMyIngameHUD::ShowPopupClear(const bool bHasAuthority) const
{
	PopupClear->SetVisibility(ESlateVisibility::Visible);
	PopupClear->Initialize(bHasAuthority);
	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->bShowMouseCursor = true;
}
