#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PopupEnemyShip.generated.h"

class UProgressBar;

UCLASS()
class CAPSTONE_2024_20_API UPopupEnemyShip : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UProgressBar* EnemyShipHPProgressBar;
	
	void SetEnemyShipHPProgressBarPercent(float Percent) const;
};
