// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomPasswordInputPopupWidget.h"

void URoomPasswordInputPopupWidget::SetActive(bool IsActive)
{
	if (IsActive == true)
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		SetVisibility(ESlateVisibility::Visible);
	}
}
