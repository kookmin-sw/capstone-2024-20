// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeNPCWidgetComponent.h"
#include "UpgradeNameWidget.h"

UUpgradeNPCWidgetComponent::UUpgradeNPCWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	static ConstructorHelpers::FClassFinder<UUserWidget>
		BP_WidgetFinder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/WidgetBlueprints/UpgradeNPC_NamePlateWidget.UpgradeNPC_NamePlateWidget_C'"));

	if (BP_WidgetFinder.Succeeded())
	{
		SetWidgetClass(BP_WidgetFinder.Class);
		SetWidgetSpace(EWidgetSpace::Screen);
	}
}

void UUpgradeNPCWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	NameWidget = Cast<UUpgradeNameWidget>(GetWidget());
}