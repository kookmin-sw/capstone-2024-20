#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PopupEnemy.generated.h"

class UProgressBar;

UCLASS()
class CAPSTONE_2024_20_API UPopupEnemy : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	UProgressBar* HPProgressBar;

	void SetHPProgressBarPercent(const float Percent) const;
};
