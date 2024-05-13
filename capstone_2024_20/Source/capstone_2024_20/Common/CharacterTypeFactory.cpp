// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterTypeFactory.h"

#include "capstone_2024_20/CapCharacterSkeletalMeshComponent.h"

UCapCharacterSkeletalMeshComponent* UCharacterTypeFactory::GetCharacter(ECharacterType Type)
{
	if (Type == ECharacterType::Character1)
	{
		return LoadSkeletalMeshComponent(FCharacterTypePath::Character1);
	}
	if (Type == ECharacterType::Character2)
	{
		return LoadSkeletalMeshComponent(FCharacterTypePath::Character2);
	}

	return nullptr;
}

UCapCharacterSkeletalMeshComponent* UCharacterTypeFactory::LoadSkeletalMeshComponent(const FString& Path)
{
	UClass* BPClass = StaticLoadClass(UObject::StaticClass(), nullptr, *Path);
	if (BPClass)
	{
		UObject* DefaultObject = BPClass->GetDefaultObject();
		if (DefaultObject)
		{
			return Cast<UCapCharacterSkeletalMeshComponent>(DefaultObject);
		}
	}

	return nullptr;
}
