// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameRoomInfoWidget.h"
#include "Components/TextBlock.h"


void UInGameRoomInfoWidget::InitSetText(const FString& Name, const FString& Code)
{
	NameText->SetText(FText::FromString(Name));
	CodeText->SetText(FText::FromString(Code));
}

void UInGameRoomInfoWidget::Show()
{
	PlayAnimationForward(InfoAnimation);
}

void UInGameRoomInfoWidget::Hide()
{
	PlayAnimationReverse(InfoAnimation);
}
