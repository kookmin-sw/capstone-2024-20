// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "MyAudioInstance.generated.h"

enum ECharacterType : int;
/**
 * 
 */
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
};
