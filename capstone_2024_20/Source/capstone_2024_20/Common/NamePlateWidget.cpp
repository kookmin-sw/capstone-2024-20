// Fill out your copyright notice in the Description page of Project Settings.


#include "NamePlateWidget.h"

#include "Components/TextBlock.h"

void UNamePlateWidget::SetName(FString Name)
{
	if(NameText)
	{
		NameText->SetText(FText::FromString(Name));
	}
}
