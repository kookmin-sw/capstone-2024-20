// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MappingContextSwitcher.generated.h"

class UInputMappingContext;
class UEnhancedInputLocalPlayerSubsystem;
/**
 * 
 */
UCLASS()
class CAPSTONE_2024_20_API UMappingContextSwitcher : public UObject
{
	GENERATED_BODY()
private:
	TArray<UInputMappingContext*> InputMappingContexts;

public:
	UEnhancedInputLocalPlayerSubsystem* Subsystem;

	void Init(APlayerController* PlayerController);

	void ReplaceMappingContext(TArray<UInputMappingContext*> NewMapingContexts);
};
