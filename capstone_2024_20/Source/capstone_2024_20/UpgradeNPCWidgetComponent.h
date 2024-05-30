// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UpgradeNameWidget.h"
#include "Components/WidgetComponent.h"
#include "UpgradeNPCWidgetComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CAPSTONE_2024_20_API UUpgradeNPCWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	UUpgradeNPCWidgetComponent();
	
	UPROPERTY()
	UUpgradeNameWidget* NameWidget;

protected:
	virtual void BeginPlay() override;

};
