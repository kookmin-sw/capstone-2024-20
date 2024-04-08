// Fill out your copyright notice in the Description page of Project Settings.


#include "NamePlateWidgetComponent.h"

#include "NamePlateWidget.h"


UNamePlateWidgetComponent::UNamePlateWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	static ConstructorHelpers::FClassFinder<UUserWidget>
		BP_WidgetFinder(TEXT("/Game/WidgetBlueprints/Common/BP_NamePlateWidget.BP_NamePlateWidget_C"));

	if (BP_WidgetFinder.Succeeded())
	{
		SetWidgetClass(BP_WidgetFinder.Class);
		SetRelativeLocation(FVector(0, 0, 60.0f));
		SetWidgetSpace(EWidgetSpace::Screen);
	}
}

template <class T>
T UNamePlateWidgetComponent::ChangeWidget(FString& Path)
{
	UUserWidget* Widget = ChangeWidget(Path);
	return Cast<T>(Widget);
}


UUserWidget* UNamePlateWidgetComponent::ChangeWidget(FString& Path)
{
	UClass* BlueprintClass = LoadClass<UUserWidget>(nullptr, *Path);

	if (BlueprintClass)
	{
		UUserWidget* NewWidget = CreateWidget<UUserWidget>(GetWorld(),
		                                                   BlueprintClass);

		return ChangeWidget(NewWidget);
	}

	return nullptr;
}

UUserWidget* UNamePlateWidgetComponent::ChangeWidget(UUserWidget* NewWidget)
{
	SetWidget(NewWidget);

	return NewWidget;
}