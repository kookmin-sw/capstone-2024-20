// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadyCharacterWidget.h"

#include "Components/TextBlock.h"

void UReadyCharacterWidget::ChangeColor(FLinearColor color)
{
	ReadyImage->SetColorAndOpacity(color);
}

void UReadyCharacterWidget::SetVisibilityFromBool(bool IsVisible)
{
	if (IsVisible == true)
	{
		ReadyImage->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ReadyImage->SetVisibility(ESlateVisibility::Hidden);
	}
}
