// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomPasswordInputPopupWidget.h"

#include "Components/Button.h"

void URoomPasswordInputPopupWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CloseButton->OnClicked.AddDynamic(this, &ThisClass::OnCloseButtonClicked);
}

void URoomPasswordInputPopupWidget::OnCloseButtonClicked()
{
	SetActive(false);
}

void URoomPasswordInputPopupWidget::SetActive(bool IsActive)
{
	if (IsActive == true)
	{
		SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}
