// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyObject.h"
#include "Components/ArrowComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "MyBed.generated.h"


/**
 * 
 */
UCLASS()
class CAPSTONE_2024_20_API AMyBed : public AMyObject
{
	GENERATED_BODY()

	AMyBed();

protected:
	UPROPERTY(VisibleAnywhere, Category="Sound")
	USoundCue* SleepSoundCue;

	UAudioComponent* SleepAudioComponent;
	
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	AActor* Camera_Bed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Directional")
	UArrowComponent* SleepArrow;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Directional")
	UArrowComponent* AwakeArrow;

	UFUNCTION()
	FVector GetSleepLocation();
	UFUNCTION()
	FRotator GetSleepRotation();

	UFUNCTION()
	FVector GetAwakeLocation();
	UFUNCTION()
	FRotator GetAwakeRotation();

	UFUNCTION()
	void SleepSound();

	UFUNCTION()
	void AwakeSound();
};
