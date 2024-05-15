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
	virtual void NativeConstruct() override;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* CostText;
	
	UFUNCTION()
	void OnClickUpgradeButton();
};



