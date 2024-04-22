// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ICapPawn.generated.h"

class UInputMappingContext;
/**
 * 
 */
UINTERFACE(MinimalAPI)
class UCapPawn : public UInterface
{
	GENERATED_BODY()
};

class CAPSTONE_2024_20_API ICapPawn
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual UInputMappingContext* GetMappingContext() = 0;
};
