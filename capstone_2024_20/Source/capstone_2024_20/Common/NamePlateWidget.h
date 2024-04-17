// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "NamePlateWidget.generated.h"

UCLASS()
class CAPSTONE_2024_20_API UNamePlateWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* NameText;

public:
	void SetName(FString Name);
};
