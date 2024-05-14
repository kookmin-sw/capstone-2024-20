#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "MyIngameHUD.generated.h"

class UPopupDead;

UCLASS()
// ReSharper disable once IdentifierTypo
class CAPSTONE_2024_20_API AMyIngameHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	void SetPopupDeadVisibility(bool bIsVisible) const;

protected:
	// ! Set on blueprint
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	// ReSharper disable once IdentifierTypo
	TSubclassOf<UUserWidget> IngameWidgetClass;

	// ! Set on blueprint
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	TSubclassOf<UPopupDead> PopupDeadClass;

private:
	UPROPERTY()
	UUserWidget* InGameWidget;

	UPROPERTY()
	UPopupDead* PopupDead;
};
