// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "capstone_2024_20/CharacterType.h"
#include "UObject/Object.h"
#include "CharacterTypeFactory.generated.h"

class UCapCharacterSkeletalMeshComponent;

UCLASS()
class CAPSTONE_2024_20_API UCharacterTypeFactory : public UObject
{
	GENERATED_BODY()

public:
	static UCapCharacterSkeletalMeshComponent* LoadSkeletalMeshComponent(const FString& Path);
	static UCapCharacterSkeletalMeshComponent* GetCharacter(ECharacterType Type);

private:
};
