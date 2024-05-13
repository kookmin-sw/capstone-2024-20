#include "EnemyShipCannonBall.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "../MyShip.h"

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

	AMyShip* MyShip = Cast<AMyShip>(OtherActor);
	if(MyShip == nullptr)
	{
		return;
	}
	
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WaterSplashEffect, Hit.ImpactPoint, FRotator::ZeroRotator, WaterSplashEffectScale);
	
	MyShip->Damage(1); // Todo@autumn - This is a temporary solution, replace it with data.
	
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::DestroyWithDelay, DestroyDelayTime, false);
}

void AEnemyShipCannonBall::DestroyWithDelay()
{
	if (HasAuthority())
	{
		Destroy();
	}
}
