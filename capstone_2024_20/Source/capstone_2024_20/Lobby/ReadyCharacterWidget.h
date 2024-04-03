// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "ReadyCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONE_2024_20_API UReadyCharacterWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta=(BindWidget))
	UImage* ReadyImage;
public:
	void ChnageColor(FLinearColor color);

	void SetVisibilityFromBool(bool IsVisible);
};
