// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionWidgetComponent.h"

#include "RoundProgressWidget.h"


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
	GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Emerald, TEXT("StartProgressBar"));
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, Time, &TimerHandle]()
	{
		float ElapsedTime = GetWorld()->GetDeltaSeconds();
		float CurrentValue = FMath::Clamp(RoundProgressWidget->GetPercent() + (ElapsedTime/Time), 0.0f, 1.0f);
		GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, FString::Printf(TEXT("currentValue: %f,,,,, %f"), CurrentValue, GetWorld()->GetDeltaSeconds()));
		if (CurrentValue >= Time)
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		}

		RoundProgressWidget->SetPercent(CurrentValue);
	}, 0.01f, true, 0.1f);
}

