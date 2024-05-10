#include "EnemyShipCannonBall.h"
#include "Kismet/GameplayStatics.h"

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
	if (WaterSplashEffect)
	{
		const FVector Scale(3.0f, 3.0f, 3.0f);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WaterSplashEffect, Hit.ImpactPoint, FRotator(0.0f), Scale);
	}
}
