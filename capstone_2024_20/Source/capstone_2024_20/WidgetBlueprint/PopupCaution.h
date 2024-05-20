#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PopupCaution.generated.h"

class UTextBlock;

UCLASS()
class CAPSTONE_2024_20_API UPopupCaution : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* CautionText;

	void SetCautionText(const FText& Text) const;
};
