// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeWidgetElement.h"

#include "Components/Button.h"

void UUpgradeWidgetElement::NativeConstruct()
{
	Super::NativeConstruct();
	UpgradeButton->OnClicked.AddDynamic(this, &ThisClass::OnClickUpgradeButton);
}

void UUpgradeWidgetElement::OnClickUpgradeButton()
{
	OnClickUpgradeDelegate.Broadcast();
}
