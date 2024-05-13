// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterChangeWidget.generated.h"

class UButton;/**
 * 
 */
UCLASS()
class CAPSTONE_2024_20_API UCharacterChangeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UButton* NextButton;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UButton* PrevButton;
};
