#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PopupClear.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class CAPSTONE_2024_20_API UPopupClear : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TextBlockWaitingServer;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Button_0;

	void Initialize(bool bHasAuthority) const;
};
