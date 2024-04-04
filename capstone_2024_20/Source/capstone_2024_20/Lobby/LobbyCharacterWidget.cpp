// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyCharacterWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void ULobbyCharacterWidget::ChangeColor(FLinearColor color)
{
	ReadyImage->SetColorAndOpacity(color);
}
void ULobbyCharacterWidget::SetName(FString Name)
{
	NameText->SetText(FText::FromString(Name));
}

void ULobbyCharacterWidget::SetVisibilityFromBool(bool IsVisible)
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