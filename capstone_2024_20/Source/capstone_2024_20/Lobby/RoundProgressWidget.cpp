// Fill out your copyright notice in the Description page of Project Settings.


#include "RoundProgressWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void URoundProgressWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	RoundProgressBar = Image->GetDynamicMaterial();
	SetPercent(Percent);
}

void URoundProgressWidget::SetPercent(float NewPercent)
{
	if(IsValid(RoundProgressBar) == false)
	{
		RoundProgressBar = Image->GetDynamicMaterial();
	}
	
	RoundProgressBar->SetScalarParameterValue(FName(TEXT("Percent")), NewPercent);
}

void URoundProgressWidget::SetKeyText(FString& NewText)
{
	KeyText->SetText(FText::FromString(NewText));
}

void URoundProgressWidget::SetExplainText(FString& NewText)
{
	ExplainText->SetText(FText::FromString(NewText));
}
