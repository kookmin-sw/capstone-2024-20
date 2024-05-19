// Fill out your copyright notice in the Description page of Project Settings.


#include "RoundProgressWidget.h"

#include "Components/Image.h"

void URoundProgressWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	RoundProgressBar = Image->GetDynamicMaterial();
	SetPercent(Percent);
}

void URoundProgressWidget::SetPercent(float NewPercent)
{
	Percent = NewPercent;
	if (IsValid(RoundProgressBar) == false)
	{
		RoundProgressBar = Image->GetDynamicMaterial();
	}

	RoundProgressBar->SetScalarParameterValue(FName(TEXT("Percent")), NewPercent);
}

float URoundProgressWidget::GetPercent()
{
	return Percent;
}
