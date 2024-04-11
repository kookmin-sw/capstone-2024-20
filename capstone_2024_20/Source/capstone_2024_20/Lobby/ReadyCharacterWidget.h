// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ILobbyReadyWidget.h"
#include "Blueprint/UserWidget.h"
#include "capstone_2024_20/Common/NamePlateWidget.h"
#include "Components/Image.h"
#include "ReadyCharacterWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class CAPSTONE_2024_20_API UReadyCharacterWidget : public UNamePlateWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta=(BindWidget))
	UImage* ReadyImage;
	
public:
	void ChangeColor(FLinearColor color);

	void SetVisibilityFromBool(bool IsVisible);
};
