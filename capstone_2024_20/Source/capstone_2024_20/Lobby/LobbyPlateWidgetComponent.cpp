// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlateWidgetComponent.h"

#include "ReadyCharacterWidget.h"


ULobbyPlateWidgetComponent::ULobbyPlateWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULobbyPlateWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	LobbyCharacterWidget = Cast<UReadyCharacterWidget>(GetWidget());
}

void ULobbyPlateWidgetComponent::ChangeColor(FLinearColor color)
{
	LobbyCharacterWidget->ChangeColor(color);
}

void ULobbyPlateWidgetComponent::SetVisibilityFromBool(bool IsVisible)
{
	LobbyCharacterWidget->SetVisibilityFromBool(IsVisible);
}
