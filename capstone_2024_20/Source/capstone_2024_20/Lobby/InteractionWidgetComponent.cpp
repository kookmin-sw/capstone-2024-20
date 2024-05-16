// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionWidgetComponent.h"

#include "../Common/RoundProgressWidget.h"


UInteractionWidgetComponent::UInteractionWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UInteractionWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	GetWidget()->SetVisibility(ESlateVisibility::Hidden);
	RoundProgressWidget = Cast<URoundProgressWidget>(GetWidget());
}


void UInteractionWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInteractionWidgetComponent::Show(FString& Key, FString& Explain)
{
	RoundProgressWidget->SetKeyText(Key);
	RoundProgressWidget->SetExplainText(Explain);
	RoundProgressWidget->SetPercent(0);
	RoundProgressWidget->SetVisibility(ESlateVisibility::Visible);
}

void UInteractionWidgetComponent::Hide()
{
	RoundProgressWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UInteractionWidgetComponent::StartProgressBar(float Time)
{
	if(GetWorld()->GetTimerManager().IsTimerActive(ProgressTimerHandle) == true)
		return;
	
	GetWorld()->GetTimerManager().SetTimer(ProgressTimerHandle, [this, Time]()
	{
		float ElapsedTime = GetWorld()->GetDeltaSeconds();
		float CurrentValue = FMath::Clamp(RoundProgressWidget->GetPercent() + (ElapsedTime/Time), 0.0f, 1.0f);

		if (CurrentValue >= Time)
		{
			GetWorld()->GetTimerManager().ClearTimer(ProgressTimerHandle);
		}

		RoundProgressWidget->SetPercent(CurrentValue);
	}, 0.01f, true, 0.1f);
}

void UInteractionWidgetComponent::StopProgressBar()
{
	if(GetWorld()->GetTimerManager().IsTimerActive(ProgressTimerHandle) == true)
	{
		RoundProgressWidget->SetPercent(0.0f);
		GetWorld()->GetTimerManager().ClearTimer(ProgressTimerHandle);
	}
}

