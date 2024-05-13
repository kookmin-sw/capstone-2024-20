// Fill out your copyright notice in the Description page of Project Settings.


#include "MyIngameHUD.h"



void AMyIngameHUD::BeginPlay()
{
	Super::BeginPlay();

	if (IngameWidgetClass)
	{
		IngameWidget = CreateWidget<UUserWidget>(GetWorld(), IngameWidgetClass);
		if (IngameWidget)
		{
			IngameWidget->AddToViewport();
		}
	}
}
