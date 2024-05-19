#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UpgradeWidget.generated.h"

class UImage;
class UButton;
class UUpgradeWidgetElement;

UCLASS()
class CAPSTONE_2024_20_API UUpgradeWidget : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UUpgradeWidgetElement* SpeedUpgrade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UUpgradeWidgetElement* HandlingUpgrade;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta=(BindWidget))
	UUpgradeWidgetElement* CannonAttackUpgrade;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta=(BindWidget))
	UButton* CloseButton;

public:
	UFUNCTION()
	void SetVisibilityWithBool(bool IsActive);

private:
	UFUNCTION()
	void OnClickedCloseButton();
};
