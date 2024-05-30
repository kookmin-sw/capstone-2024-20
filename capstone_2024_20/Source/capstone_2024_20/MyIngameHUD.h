#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "Upgrade/UpgradeWidget.h"
#include "MyIngameHUD.generated.h"

class UPopupClear;
class UPopupGameOver;
class UPopupEnemyShip;
class UPopupCaution;
class UProgressBar;
class UPopupDead;

UCLASS()
// ReSharper disable once IdentifierTypo
class CAPSTONE_2024_20_API AMyIngameHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	// [begin] PopupDead
	void SetPopupDeadVisibility(bool bIsVisible) const;
	void SetPopupDeadTextByReviveCooldown(float ReviveCooldown) const;
	// [end] PopupDead

	// [begin] PopupUpgrade
	UUpgradeWidget* GetPopupUpgrade() const;
	void SetPopupUpgradeVisibility(bool bIsVisible) const;
	// [end] PopupUpgrade

	// [begin] EnemyShipUI
	bool GetPopupEnemyShipVisibility() const;
	void SetPopupEnemyShipVisibility(bool bIsVisible) const;
	void SetEnemyShipHPProgressBarPercent(float Percent) const;
	// [end] EnemyShipUI

	// [begin] PopupCaution
	void ShowPopupCaution(const FText& Text) const;
	// [end] PopupCaution

	// [begin] PopupGameOver
	void ShowPopupGameOver(bool bHasAuthority) const;
	// [end] PopupGameOver

	// [begin] PopupClear
	void ShowPopupClear(const bool bHasAuthority) const;
	// [end] PopupClear

protected:
	// ! Set on blueprint
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	// ReSharper disable once IdentifierTypo
	TSubclassOf<UUserWidget> IngameWidgetClass;

	// ! Set on blueprint
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	TSubclassOf<UPopupDead> PopupDeadClass;

	// ! Set on blueprint
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUpgradeWidget> PopupUpgradeClass;

	TSubclassOf<UPopupCaution> PopupCautionClass;
	TSubclassOf<UPopupEnemyShip> PopupEnemyShipClass;

	TSubclassOf<UPopupGameOver> PopupGameOverClass;
	TSubclassOf<UPopupClear> PopupClearClass;

private:
	UPROPERTY()
	UUserWidget* InGameWidget;

	UPROPERTY()
	UPopupDead* PopupDead;

	UPROPERTY()
	UUpgradeWidget* PopupUpgrade;

	UPROPERTY()
	UPopupEnemyShip* PopupEnemyShip;

	UPROPERTY()
	UPopupCaution* PopupCaution;

	UPROPERTY()
	UPopupGameOver* PopupGameOver;

	UPROPERTY()
	UPopupClear* PopupClear;

	// ! 일부 클래스에서, HUD의 BeginPlay가 늦게 호출되는 경우가 있어 해당 클래스에서는 BeginPlay를 직접 호출.
	// ! 이때, BeginPlay가 이미 호출된 경우에는 다시 호출하지 않도록 예외 처리하기 위한 변수
	bool bIsCalledBeginPlay = false;
};
