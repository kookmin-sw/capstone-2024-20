#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RoundProgressWidget.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class CAPSTONE_2024_20_API URoundProgressWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativePreConstruct() override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstanceDynamic* RoundProgressBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Percent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UImage* Image;

	
	
	UFUNCTION()
	void SetPercent(float NewPercent);

	UFUNCTION()
	float GetPercent();
};
