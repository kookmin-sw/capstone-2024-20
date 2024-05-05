// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBed.h"


#include "Kismet/GameplayStatics.h"

AMyBed::AMyBed()
{


	
	SleepAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("MyAudioComponent"));
	SleepAudioComponent->bAutoActivate = false;  // 자동 재생 비활성화


	static ConstructorHelpers::FObjectFinder<USoundCue> SoundCueFinder(TEXT("/Script/Engine.SoundCue'/Game/Sounds/Character/SnoringSQ.SnoringSQ'"));
	if (SoundCueFinder.Succeeded())
	{
		SleepSoundCue = SoundCueFinder.Object;
	}
	
	// 필요한 설정 적용
	SleepAudioComponent->SetSound(Cast<USoundBase>(SleepSoundCue));
}


FVector AMyBed::GetSleepLocation()
{
	return SleepArrow->GetComponentLocation();
}

FRotator AMyBed::GetSleepRotation()
{
	return SleepArrow->GetComponentRotation();
}

FVector AMyBed::GetAwakeLocation()
{
	return AwakeArrow->GetComponentLocation();
}

FRotator AMyBed::GetAwakeRotation()
{
	return AwakeArrow->GetComponentRotation();
}

void AMyBed::SleepSound()
{
	SleepAudioComponent->Play();
}

void AMyBed::AwakeSound()
{
	SleepAudioComponent->Stop();
}




