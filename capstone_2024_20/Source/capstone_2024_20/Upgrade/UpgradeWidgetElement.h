#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UpgradeWidgetElement.generated.h"


class UButton;
class UTextBlock;

UCLASS()
class CAPSTONE_2024_20_API UUpgradeWidgetElement : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* NameText;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* LevelText;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* ExplainText;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UButton* UpgradeButton;
};
