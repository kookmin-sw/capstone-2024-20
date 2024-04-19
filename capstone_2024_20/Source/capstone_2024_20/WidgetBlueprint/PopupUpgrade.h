#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PopupUpgrade.generated.h"

UCLASS()
class CAPSTONE_2024_20_API UPopupUpgrade : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Upgrade() const;
};
