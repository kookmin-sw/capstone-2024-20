// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeWidgetElement.h"

#include "Components/Button.h"

void UUpgradeWidgetElement::Upgrade()
{
	if(IsLevelOverMax() == true)
		return;
	
	OnClickUpgradeDelegate.Broadcast();
	CurrentLevel++;

	if(IsLevelOverMax() == true)
	{
		UpgradeButton->SetColorAndOpacity(FLinearColor::Gray);
	}
}

bool UUpgradeWidgetElement::IsLevelOverMax() const
{
	return CurrentLevel >= MaxLevel;
}
