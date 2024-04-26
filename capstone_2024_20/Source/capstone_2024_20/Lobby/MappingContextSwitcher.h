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
class CAPSTONE_2024_20_API UMappingContextSwitcher : public UActorComponent
{
	GENERATED_BODY()

	UMappingContextSwitcher();

public:
	UPROPERTY()
	TArray<UInputMappingContext*> InputMappingContexts;

	void ClearPrevMappingContext();
	bool CheckSubsystemNull();

public:
	UPROPERTY()
	UEnhancedInputLocalPlayerSubsystem* Subsystem;

	void ReplaceMappingContext(UInputMappingContext* NewMappingContext);
	void ReplaceMappingContext(TArray<UInputMappingContext*> NewMappingContexts);
};
