#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PopupEvent.generated.h"

class UProgressBar;

UCLASS()
class CAPSTONE_2024_20_API UPopupEvent : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UProgressBar* ProgressBar;

	void SetDamageCoolDownProgressBarPercent(const float Percent) const;
};
