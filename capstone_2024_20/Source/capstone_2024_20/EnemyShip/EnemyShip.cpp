﻿#include "EnemyShip.h"
#include "../MyShip.h"
#include "../Enemy/Enemy.h"
#include "Particles/ParticleSystem.h"
#include "capstone_2024_20/Object/EnemyShipCannonBall.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

AEnemyShip::AEnemyShip()
{
	SetMaxHP(2);
	SetCurrentHP(2);
}

void AEnemyShip::BeginPlay()
{
	Super::BeginPlay();

	ProjectileSpawnPoint = FindComponentByClass<UArrowComponent>();
	ProjectileClass = AEnemyShipCannonBall::StaticClass();
	FireEffect = LoadObject<UParticleSystem>(nullptr, TEXT("/Script/Engine.ParticleSystem'/Game/Particles/Realistic_Starter_VFX_Pack_Vol2/Particles/Explosion/P_Explosion_Big_A.P_Explosion_Big_A'"));
	CannonSoundCue = LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/Sounds/Cannon/CannonSQ.CannonSQ'"));
}

void AEnemyShip::LookAtMyShip(const AMyShip* MyShip)
{
	MultiCastRPC_LookAtMyShip(MyShip);
}

void AEnemyShip::MultiCastRPC_LookAtMyShip_Implementation(const AMyShip* MyShip)
{
	const auto Direction = MyShip->GetActorLocation() - GetActorLocation();
	const auto LookAtRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	SetActorRotation(LookAtRotation);
}

AEnemy* AEnemyShip::SpawnEnemy(AActor* MyShip, const float DeltaTime) const
{
	SpawnEnemyTimer += DeltaTime;

	// Todo@autumn - This is a temporary solution, replace it with data.
	if (SpawnEnemyTimer < 5.0f) 
	{
		return nullptr;
	}

	const auto MyShipLocation = MyShip->GetActorLocation();
	if (const auto Direction = MyShipLocation - GetActorLocation(); Direction.Size() > 12000.0f) // Todo@autumn - This is a temporary solution, replace it with data.
	{
		return nullptr;
	}

	// Todo@autumn - This is a temporary solution, replace it with data.
	const auto RandomX = FMath::RandRange(-100.0f, 100.0f);
	const auto RandomY = FMath::RandRange(-100.0f, 100.0f);
	const auto RandomLocation = FVector(RandomX, RandomY, 880.0f);

	AEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AEnemy>(AEnemy::StaticClass(), FTransform(UE::Math::TVector<double>(0, 0, 0)));
	SpawnedEnemy->AttachToActor(MyShip, FAttachmentTransformRules::KeepRelativeTransform);
	SpawnedEnemy->SetActorRelativeLocation(RandomLocation);

	SpawnEnemyTimer = 0.0f;

	return SpawnedEnemy;
}

bool AEnemyShip::CanSpawnEnemy() const
{
	return bCanSpawnEnemy;
}

bool AEnemyShip::CanFireCannon() const
{
	return bCanFireCannon;
}

void AEnemyShip::FireCannon(const float DeltaTime)
{
	FireCannonTimer += DeltaTime;

	if (FireCannonTimer < 10.0f) // Todo@autumn - This is a temporary solution, replace it with data.
	{
		return;
	}
	
	MultiCastRPC_FireCannon();

	FireCannonTimer = 0.0f;
}

void AEnemyShip::MultiCastRPC_FireCannon_Implementation()
{
	
	const auto SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
	const auto SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();
	
	if(HasAuthority())
	GetWorld()->SpawnActor<AEnemyShipCannonBall>(ProjectileClass, SpawnLocation, SpawnRotation);

	UGameplayStatics::PlaySoundAtLocation(this, CannonSoundCue, GetActorLocation());
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireEffect, SpawnLocation, SpawnRotation);
}

void AEnemyShip::Die()
{
	IHP::Die();
	EnemyShipDieDelegate.Execute(this);
}
