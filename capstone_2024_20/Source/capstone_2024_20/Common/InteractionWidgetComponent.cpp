// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionWidgetComponent.h"

#include "../Common/RoundProgressWidget.h"


UInteractionWidgetComponent::UInteractionWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UUserWidget>
	B_WidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/WidgetBlueprints/Common/BP_RoundProgressWidget.BP_RoundProgressWidget_C'"));
	if (B_WidgetClass.Succeeded())
	{
		SetWidgetClass(B_WidgetClass.Class);
	}

	SetWidgetSpace(EWidgetSpace::Screen);
	SetDrawSize(FVector2d(400.0f, 400.0f));
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

void UInteractionWidgetComponent::Show()
{
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
	PrevTimeSeconds = GetWorld()->GetTimeSeconds();
	GetWorld()->GetTimerManager().SetTimer(ProgressTimerHandle, [this, Time]()
	{
		const double ElapsedTime = GetWorld()->GetTimeSeconds();
		const float CurrentValue = FMath::Clamp(RoundProgressWidget->GetPercent() + ((ElapsedTime-PrevTimeSeconds)/Time), 0.0f, 1.0f);
		PrevTimeSeconds = ElapsedTime;
		if (CurrentValue >= Time)
		{
			GetWorld()->GetTimerManager().ClearTimer(ProgressTimerHandle);
		}

		RoundProgressWidget->SetPercent(CurrentValue);
	}, 0.01f, true, 0.0f);
}

void UInteractionWidgetComponent::StopProgressBar()
{
	if(GetWorld()->GetTimerManager().IsTimerActive(ProgressTimerHandle) == true)
	{
		RoundProgressWidget->SetPercent(0.0f);
		GetWorld()->GetTimerManager().ClearTimer(ProgressTimerHandle);
	}
}

