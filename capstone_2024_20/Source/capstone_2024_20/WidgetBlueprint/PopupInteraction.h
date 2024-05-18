#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PopupInteraction.generated.h"

class UTextBlock;

UCLASS()
class CAPSTONE_2024_20_API UPopupInteraction : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InteractionText;

	void SetInteractionText(const FString& Text) const;
};
