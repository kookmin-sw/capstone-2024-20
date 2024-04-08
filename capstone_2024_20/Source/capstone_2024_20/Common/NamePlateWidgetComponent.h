// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "NamePlateWidgetComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CAPSTONE_2024_20_API UNamePlateWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UNamePlateWidgetComponent();
	
	template< class T >
	T ChangeWidget(FString& Path);
	
	UUserWidget* ChangeWidget(FString& Path);
	UUserWidget* ChangeWidget(UUserWidget* Widget);
};
