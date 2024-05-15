#include "MyCannon.h"
#include "CannonBall.h"
#include "CannonWidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Components/SlateWrapperTypes.h"
#include "Net/UnrealNetwork.h"
#include "Sound/SoundCue.h"

AMyCannon::AMyCannon(): M_ShooterMesh(nullptr), ProjectileSpawnPoint(nullptr), FireEffect(nullptr),
                        Camera_Cannon(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	static ConstructorHelpers::FObjectFinder<USoundCue> SoundCueFinder(TEXT("/Game/Sounds/Cannon/CannonSQ.CannonSQ"));
	if (SoundCueFinder.Succeeded())
	{
		CannonSoundCue = SoundCueFinder.Object;
	}
}

void AMyCannon::BeginPlay()
{
	Super::BeginPlay();
}

// ReSharper disable once CppParameterMayBeConst
void AMyCannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyCannon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyCannon, IsLoad);
}


FVector AMyCannon::GetCannonSpawnLocation() const
{
	return ProjectileSpawnPoint->GetComponentLocation();
}

FRotator AMyCannon::GetCannonSpawnRotation() const
{
	return ProjectileSpawnPoint->GetComponentRotation();
}

void AMyCannon::FireCannon()
{
	ServerRPC_FireCannon();
}

void AMyCannon::ServerRPC_FireCannon_Implementation()
{
	ACannonBall* Ball = GetWorld()->SpawnActor<ACannonBall>(ProjectileClass, GetCannonSpawnLocation(), GetCannonSpawnRotation());
	Ball->SetDamage(1);
	
	MultiCastRPC_FireCannon();
}

void AMyCannon::MultiCastRPC_FireCannon_Implementation()
{
	TriggerEffects();
}

void AMyCannon::TriggerEffects() const
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

void AMyCannon::RotateCannon(const FRotator& NewRot)
{
	MultiCastRPC_RotateCannon(NewRot);
}

void AMyCannon::MultiCastRPC_RotateCannon_Implementation(const FRotator NewRot)
{
	M_ShooterMesh->SetRelativeRotation(NewRot);
}

bool AMyCannon::GetIsLoad() const
{
	return IsLoad;
}

void AMyCannon::SetIsLoad(const bool b)
{
	IsLoad = b;
}

void AMyCannon::VisibleWidget(bool b)
{
	WidgetComponent->SetVisibility(b);
}
