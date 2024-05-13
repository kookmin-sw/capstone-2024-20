// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterChangerWidgetComponent.h"


UCharacterChangerWidgetComponent::UCharacterChangerWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	static ConstructorHelpers::FClassFinder<UUserWidget>
		BP_WidgetFinder(TEXT("/Game/WidgetBlueprints/Lobby/BP_CharacterChangeWidget.BP_CharacterChangeWidget_C"));

	if (BP_WidgetFinder.Succeeded())
	{
		SetWidgetClass(BP_WidgetFinder.Class);
		SetWidgetSpace(EWidgetSpace::Screen);
	}
}


void UCharacterChangerWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCharacterChangerWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

