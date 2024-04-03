// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadyCharacterWidget.h"

void UReadyCharacterWidget::ChnageColor(FLinearColor color)
{
	GEngine->AddOnScreenDebugMessage(-1,
		60.0f, FColor::Red,
		TEXT("red change color"));
	ReadyImage->SetColorAndOpacity(color);
}

void UReadyCharacterWidget::SetVisibilityFromBool(bool IsVisible)
{
	if(IsVisible == true)
	{
		ReadyImage->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ReadyImage->SetVisibility(ESlateVisibility::Hidden);
	}
}
