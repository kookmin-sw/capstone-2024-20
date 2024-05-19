#include "CannonBall.h"
#include "EnemyShip/EnemyShip.h"
#include "Kismet/GameplayStatics.h"

ACannonBall::ACannonBall()
{
	PrimaryActorTick.bCanEverTick = true;
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	RootComponent = ProjectileMesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetUpdatedComponent(ProjectileMesh);
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 6000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false; // 튕겨나가지않음

	ProjectileMesh->OnComponentHit.AddDynamic(this, &ACannonBall::OnHit);
}

void ACannonBall::BeginPlay()
{
	Super::BeginPlay();
}

// ReSharper disable once CppParameterMayBeConst
void ACannonBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACannonBall::DestroyDelay()
{
	if (HasAuthority())
	{
		Destroy();
	}
}

void ACannonBall::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                        FVector NormalImpulse, const FHitResult& Hit)
{
	if (HasAuthority())
	{
		if(AEnemyShip* EnemyShip = Cast<AEnemyShip>(OtherActor))
		{
			EnemyShip->Damage(Damage);

			ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::DestroyDelay, DestroyDelayTime, false);
	}

	MultiCastRPC_OnHit(Hit);
}

void ACannonBall::MultiCastRPC_OnHit_Implementation(const FHitResult& Hit)
{
	const FVector Scale(3.0f, 3.0f, 3.0f);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WaterSplashEffect, Hit.ImpactPoint, FRotator(0.0f), Scale);
}

void ACannonBall::SetDamage(const int32 Dmg)
{
	Damage = Dmg;
}
