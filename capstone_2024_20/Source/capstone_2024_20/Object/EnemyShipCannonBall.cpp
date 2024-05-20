﻿#include "EnemyShipCannonBall.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "../MyShip.h"
#include "capstone_2024_20/MyIngameHUD.h"
#include "capstone_2024_20/Event/Event.h"
#include "capstone_2024_20/Sailing/SailingSystem.h"

AEnemyShipCannonBall::AEnemyShipCannonBall(): StaticMesh(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
	AActor::SetReplicateMovement(true);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/GameObjects/CannonBall/CannonBall.CannonBall'")));
	RootComponent = StaticMesh;
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetUpdatedComponent(StaticMesh);
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 6000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	StaticMesh->OnComponentHit.AddDynamic(this, &AEnemyShipCannonBall::OnHit);

	WaterSplashEffect = LoadObject<UParticleSystem>(nullptr, TEXT("/Script/Engine.ParticleSystem'/Game/Particles/FXVarietyPack/Particles/P_ky_waterBallHit.P_ky_waterBallHit'"));
}

void AEnemyShipCannonBall::BeginPlay()
{
	Super::BeginPlay();
	MyInGameHUD = Cast<AMyIngameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

// ReSharper disable once CppParameterMayBeConst
void AEnemyShipCannonBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyShipCannonBall::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	bool bIsShowEnemyShipHPProgressBar = false;
	
	if (HasAuthority())
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::DestroyWithDelay, DestroyDelayTime, false);
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
		AMyShip* MyShip = Cast<AMyShip>(OtherActor);
		if(MyShip == nullptr)
		{
			return;
		}
		
		MyShip->Damage(Damage);
		bIsShowEnemyShipHPProgressBar = true;

		if (CanStartFire())
		{
			StartFire(MyShip);
		}
	}

	MulticastRPC_OnHit(Hit, bIsShowEnemyShipHPProgressBar);
}

void AEnemyShipCannonBall::MulticastRPC_OnHit_Implementation(const FHitResult& Hit, const bool bIsShowEnemyShipHPProgressBar)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WaterSplashEffect, Hit.ImpactPoint, FRotator::ZeroRotator, WaterSplashEffectScale);

	if (bIsShowEnemyShipHPProgressBar && MyInGameHUD->GetPopupEnemyShipVisibility() == false)
	{
		MyInGameHUD->SetPopupEnemyShipVisibility(true);
	}
}

void AEnemyShipCannonBall::DestroyWithDelay()
{
	if (HasAuthority())
	{
		Destroy();
	}
}

bool AEnemyShipCannonBall::CanStartFire() const
{
	if(!FMath::RandBool())
	{
		return false;
	}

	TArray<AActor*> FoundFire;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEvent::StaticClass(), FoundFire);

	return FoundFire.Num() == 0;
}

void AEnemyShipCannonBall::StartFire(AMyShip* MyShip) const
{
	const FVector SpawnLocation = MyShip->GetNearestEventSpawnPointLocationFrom(GetActorLocation());
	AEvent* SpawnedEvent = GetWorld()->SpawnActor<AEvent>(AEvent::StaticClass(), FTransform(UE::Math::TVector<double>(0, 0, 0)));
	SpawnedEvent->AttachToActor(MyShip, FAttachmentTransformRules::KeepRelativeTransform);
	SpawnedEvent->SetActorLocation(SpawnLocation);

	// [begin] ! Never modify or call anything of SailingSystem instance in this class except for these lines.
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASailingSystem::StaticClass(), FoundActors);
	ASailingSystem* SailingSystem = Cast<ASailingSystem>(FoundActors[0]);
	SailingSystem->AddSpawnedEventFromEnemyShipCannonBall(SpawnedEvent);
	// [end] ! Never modify or call anything of SailingSystem instance in this class except for these lines.
}
