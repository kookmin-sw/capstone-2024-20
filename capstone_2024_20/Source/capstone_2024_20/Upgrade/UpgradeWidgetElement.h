#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UpgradeWidgetElement.generated.h"


class UButton;
class UTextBlock;

DECLARE_MULTICAST_DELEGATE(FOnClickUpgradeDelegate);

UCLASS()
class CAPSTONE_2024_20_API UUpgradeWidgetElement : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY()
	int32 MaxLevel;
	UPROPERTY()
	int32 CurrentLevel;
public:
	FOnClickUpgradeDelegate OnClickUpgradeDelegate;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* NameText;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* LevelText;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* ExplainText;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UButton* UpgradeButton;

	UFUNCTION()
	void Upgrade();

	UFUNCTION()
	bool IsLevelOverMax() const;
};



