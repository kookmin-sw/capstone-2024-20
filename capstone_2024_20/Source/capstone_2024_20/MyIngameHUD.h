#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "MyIngameHUD.generated.h"

UCLASS()
// ReSharper disable once IdentifierTypo
class CAPSTONE_2024_20_API AMyIngameHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	// ReSharper disable once IdentifierTypo
	TSubclassOf<UUserWidget> IngameWidgetClass;

private:
	UPROPERTY()
	UUserWidget *InGameWidget;
};
