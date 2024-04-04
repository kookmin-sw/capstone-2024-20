// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadyCharacterWidget.h"

#include "Components/TextBlock.h"

void UReadyCharacterWidget::ChangeColor(FLinearColor color)
{
	LobbyCharacterWidget->ChangeColor(color);
}

void UReadyCharacterWidget::SetName(FString Name)
{
	LobbyCharacterWidget->SetName(Name);
}

void UReadyCharacterWidget::SetVisibilityFromBool(bool IsVisible)
{
	LobbyCharacterWidget->SetVisibilityFromBool(IsVisible);
}
