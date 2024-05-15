#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UpgradeWidgetElement.generated.h"


class UImage;
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
	int32 CurrentLevel=0;
public:
	FOnClickUpgradeDelegate OnClickUpgradeDelegate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UImage* UpgradeImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* NameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* LevelText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* ExplainText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UButton* UpgradeButton;

	UFUNCTION()
	void Upgrade();

	UFUNCTION()
	bool IsLevelOverMax() const;
};



