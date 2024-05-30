// Fill out your copyright notice in the Description page of Project Settings.


#include "RoundProgressControllerWidget.h"

#include "capstone_2024_20/Common/RoundProgressWidget.h"


void URoundProgressControllerWidget::StartProgressBar(float Time)
{
	if(GetWorld()->GetTimerManager().IsTimerActive(ProgressTimerHandle) == true)
		return;
	PrevTimeSeconds = GetWorld()->GetTimeSeconds();
	GetWorld()->GetTimerManager().SetTimer(ProgressTimerHandle, [this, Time]()
	{
		const double ElapsedTime = GetWorld()->GetTimeSeconds();
		const float CurrentValue = FMath::Clamp(GetPercent() + ((ElapsedTime-PrevTimeSeconds)/Time), 0.0f, 1.0f);
		PrevTimeSeconds = ElapsedTime;
		if (CurrentValue >= 1.0f)
		{
			GetWorld()->GetTimerManager().ClearTimer(ProgressTimerHandle);
		}

		SetPercent(CurrentValue);
	}, 0.01f, true, 0.0f);
}

void URoundProgressControllerWidget::StopProgressBar()
{
	if(GetWorld()->GetTimerManager().IsTimerActive(ProgressTimerHandle) == true)
	{
		SetPercent(0.0f);
		GetWorld()->GetTimerManager().ClearTimer(ProgressTimerHandle);
	}
}
