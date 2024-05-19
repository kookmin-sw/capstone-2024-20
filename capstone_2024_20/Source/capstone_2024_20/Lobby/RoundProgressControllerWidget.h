// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "capstone_2024_20/Common/RoundProgressWidget.h"
#include "RoundProgressControllerWidget.generated.h"

class URoundProgressWidget;
/**
 * 
 */
UCLASS()
class CAPSTONE_2024_20_API URoundProgressControllerWidget : public URoundProgressWidget
{
	GENERATED_BODY()
	
	FTimerHandle ProgressTimerHandle;
	double PrevTimeSeconds;

public:
	
	UFUNCTION()
	void StartProgressBar(float Time);

	UFUNCTION()
	void StopProgressBar();
};
