// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerListElementWidget.h"

void ULobbyPlayerListElementWidget::ChangeColor(FLinearColor Color)
{
	Image->SetColorAndOpacity(Color);
}

void ULobbyPlayerListElementWidget::SetVisibilityFromBool(bool IsVisible)
{
	if (IsVisible == true)
	{
		Image->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Image->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ULobbyPlayerListElementWidget::Clear()
{
	Super::Clear();
	SetVisibilityFromBool(false);
}
