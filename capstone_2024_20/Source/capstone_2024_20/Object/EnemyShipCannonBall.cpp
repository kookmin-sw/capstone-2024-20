#include "EnemyShipCannonBall.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "../MyShip.h"
#include "capstone_2024_20/Event/Event.h"

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
}

// ReSharper disable once CppParameterMayBeConst
void AEnemyShipCannonBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyShipCannonBall::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WaterSplashEffect, Hit.ImpactPoint, FRotator::ZeroRotator, WaterSplashEffectScale);
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::DestroyWithDelay, DestroyDelayTime, false);
	
	AMyShip* MyShip = Cast<AMyShip>(OtherActor);
	if(MyShip == nullptr)
	{
		return;
	}
		
	MyShip->Damage(Damage);

	if (CanStartFire())
	{
		StartFire(MyShip);
	}
	
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemyShipCannonBall::DestroyWithDelay()
{
	if (HasAuthority())
	{
		Destroy();
	}
}

bool AEnemyShipCannonBall::CanStartFire()
{
	if(!FMath::RandBool())
	{
		return false;
	}

	return true;
}

void AEnemyShipCannonBall::StartFire(AMyShip* MyShip) const
{
	const FVector SpawnLocation = MyShip->GetNearestEventSpawnPointLocationFrom(GetActorLocation());
	AEvent* SpawnedEvent = GetWorld()->SpawnActor<AEvent>(AEvent::StaticClass(), FTransform(UE::Math::TVector<double>(0, 0, 0)));
	SpawnedEvent->AttachToActor(MyShip, FAttachmentTransformRules::KeepRelativeTransform);
	SpawnedEvent->SetActorLocation(SpawnLocation);
}
