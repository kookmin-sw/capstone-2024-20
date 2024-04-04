// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "ReadyCharacterWidget.generated.h"

class UTextBlock;
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

	UPROPERTY(meta=(BindWidget))
	UTextBlock* NameText;
public:
	void ChnageColor(FLinearColor color);

	void SetName(FString Name);

	void SetVisibilityFromBool(bool IsVisible);
};
