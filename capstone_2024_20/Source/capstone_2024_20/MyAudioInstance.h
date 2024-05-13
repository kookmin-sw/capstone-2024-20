// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterType.h"
#include "Engine/GameInstance.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "MyAudioInstance.generated.h"
/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FCharacterTypeDelegate, ECharacterType);
UCLASS()
class CAPSTONE_2024_20_API UMyAudioInstance : public UGameInstance
{
	GENERATED_BODY()

	
public:
	UMyAudioInstance();
	

	virtual void OnStart() override;

	
	virtual void Init() override;
	
	UFUNCTION(BlueprintCallable)
	void PlayMusic(int32 Index);

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Sound")
	TArray<USoundCue*> MusicTracks;

public:
	UPROPERTY(BlueprintReadWrite)
	UAudioComponent* MusicComponent;

private:
	ECharacterType CharacterType;
public:
	ECharacterType GetCharacterType() const;
	void SetCharacterType(ECharacterType Type);

	FCharacterTypeDelegate OnChangeCharacterTypeDelegate;
};
