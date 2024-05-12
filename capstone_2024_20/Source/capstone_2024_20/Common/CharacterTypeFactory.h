// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CharacterTypeFactory.generated.h"

/**
 * 
 */
UENUM()
enum ECharacterType
{
	Character1,
	Character2,
	Character3,
};

UCLASS()
class CAPSTONE_2024_20_API UCharacterTypeFactory : public UObject
{
	GENERATED_BODY()
};
