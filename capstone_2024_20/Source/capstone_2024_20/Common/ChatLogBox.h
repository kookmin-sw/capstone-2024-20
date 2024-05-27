// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatLogBox.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class CAPSTONE_2024_20_API UChatLogBox : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	UTextBlock* Title;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* Detail;

public:
	UFUNCTION()
	void SetTitleAndDetail(const FString& NewTitle, const FString& NewDetail);
};
