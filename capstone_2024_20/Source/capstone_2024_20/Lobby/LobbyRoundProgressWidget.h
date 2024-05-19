#pragma once

#include "CoreMinimal.h"
#include "capstone_2024_20/Common/RoundProgressWidget.h"
#include "LobbyRoundProgressWidget.generated.h"


UCLASS()
class CAPSTONE_2024_20_API ULobbyRoundProgressWidget : public URoundProgressWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* KeyText;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* ExplainText;

	UFUNCTION()
	void SetKeyText(FString& NewText);

	UFUNCTION()
	void SetExplainText(FString& NewText);
};
