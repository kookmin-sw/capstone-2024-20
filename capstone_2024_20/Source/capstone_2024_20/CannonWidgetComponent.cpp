// Fill out your copyright notice in the Description page of Project Settings.


#include "CannonWidgetComponent.h"

#include "CannonWidget.h"

UCannonWidgetComponent::UCannonWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	static ConstructorHelpers::FClassFinder<UUserWidget>
		BP_WidgetFinder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/WidgetBlueprints/Cannon_Widget.Cannon_Widget_C'"));

	if (BP_WidgetFinder.Succeeded())
	{
		SetWidgetClass(BP_WidgetFinder.Class);
		SetWidgetSpace(EWidgetSpace::Screen);
	}
}

void UCannonWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	CannonWidget = Cast<UCannonWidget>(GetWidget());
}