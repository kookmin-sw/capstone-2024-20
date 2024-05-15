// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "CannonWidgetComponent.generated.h"

/**
 * 
 */

class UCannonWidget;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CAPSTONE_2024_20_API UCannonWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	UCannonWidgetComponent();
	
	UPROPERTY()
	UCannonWidget* CannonWidget;

protected:
	virtual void BeginPlay() override;

};
