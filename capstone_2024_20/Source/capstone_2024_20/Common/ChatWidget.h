#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatWidget.generated.h"


UCLASS()
class CAPSTONE_2024_20_API UChatWidget : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativePreConstruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
public:
	UFUNCTION()
	void EnableChat();
private:
	void OnKeyEnter();
};
