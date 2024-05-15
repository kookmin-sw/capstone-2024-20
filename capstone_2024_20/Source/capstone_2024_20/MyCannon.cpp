// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCannon.h"

#include "CannonBall.h"
#include "CannonWidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Components/SlateWrapperTypes.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AMyCannon::AMyCannon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	static ConstructorHelpers::FObjectFinder<USoundCue> SoundCueFinder(TEXT("/Game/Sounds/Cannon/CannonSQ.CannonSQ"));
	if (SoundCueFinder.Succeeded())
	{
		CannonSoundCue = SoundCueFinder.Object;
	}
}


// Called when the game starts or when spawned
void AMyCannon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyCannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyCannon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyCannon, IsLoad);
}


FVector AMyCannon::GetCannonSpawnLocation()
{
	return ProjectileSpawnPoint->GetComponentLocation();
}

FRotator AMyCannon::GetCannonSpawnRotation()
{
	return ProjectileSpawnPoint->GetComponentRotation();
}

void AMyCannon::FireCannon()
{
	MultiCastRPC_FireCannon();
}

void AMyCannon::MultiCastRPC_FireCannon_Implementation()
{
	// 발사 위치와 방향 설정

	// 발사체 생성
	if (HasAuthority())
	{
		ACannonBall* ball = GetWorld()->SpawnActor<ACannonBall>(ProjectileClass, GetCannonSpawnLocation(), GetCannonSpawnRotation());
		ball->SetDamage(1);
	}
	// FTimerHandle EffectTimerHandle;
	// GetWorld()->GetTimerManager().SetTimer(EffectTimerHandle, this, &AMyCannon::TriggerEffects, 2.0f, false);
	TriggerEffects();
}

void AMyCannon::TriggerEffects()
{
	if (FireEffect)
	{
		if (CannonSoundCue)
		{
			UGameplayStatics::PlaySoundAtLocation(this, CannonSoundCue, GetActorLocation());
		}
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireEffect, GetCannonSpawnLocation(), GetCannonSpawnRotation());
	}
}


void AMyCannon::RotateCannon(FRotator newRot)
{
	MultiCastRPC_RotateCannon(newRot);
}

void AMyCannon::MultiCastRPC_RotateCannon_Implementation(FRotator newRot)
{
	M_ShooterMesh->SetRelativeRotation(newRot);
}

bool AMyCannon::GetIsLoad()
{
	return IsLoad;
}

void AMyCannon::SetIsLoad(bool b)
{
	IsLoad = b;
}

void AMyCannon::VisibleWidget(bool b)
{
	WidgetComponent->SetVisibility(b);
}





