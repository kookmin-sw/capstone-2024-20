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
		float ElapsedTime = GetWorld()->GetTimeSeconds();
		float CurrentValue = FMath::Lerp(0, 1, FMath::Clamp(ElapsedTime / Time, 0.0f, 1.0f));
		GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, FString::Printf(TEXT("currentValue: %f"), CurrentValue));
		if (ElapsedTime >= Time)
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		}

		RoundProgressWidget->SetPercent(CurrentValue);
	}, 0.0f, true, 0.1f);
}

