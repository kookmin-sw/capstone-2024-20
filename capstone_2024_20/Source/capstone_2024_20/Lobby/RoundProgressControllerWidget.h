// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RoundProgressControllerWidget.generated.h"

class URoundProgressWidget;
/**
 * 
 */
UCLASS()
class CAPSTONE_2024_20_API URoundProgressControllerWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	URoundProgressWidget* RoundProgressWidget;

	FTimerHandle ProgressTimerHandle;
	double PrevTimeSeconds;

public:
	template< class T >
	T* GetRoundProgressWidget() const
	{
		return Cast<T>(RoundProgressWidget);
	}
	
	UFUNCTION()
	void StartProgressBar(float Time);

	UFUNCTION()
	void StopProgressBar();
};
