#include "EnemyShip.h"
#include "../MyShip.h"
#include "../Enemy/Enemy.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "capstone_2024_20/Enemy/EnemySpawnPoint.h"
#include "Particles/ParticleSystem.h"
#include "capstone_2024_20/Object/EnemyShipCannonBall.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

AEnemyShip::AEnemyShip()
{
	AActor::SetReplicateMovement(true);
}

void AEnemyShip::BeginPlay()
{
	Super::BeginPlay();

	ProjectileSpawnPoint = FindComponentByClass<UArrowComponent>();
	ProjectileClass = AEnemyShipCannonBall::StaticClass();
	FireEffect = LoadObject<UParticleSystem>(nullptr, TEXT("/Script/Engine.ParticleSystem'/Game/Particles/Realistic_Starter_VFX_Pack_Vol2/Particles/Explosion/P_Explosion_Big_A.P_Explosion_Big_A'"));
	CannonSoundCue = LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/Sounds/Cannon/CannonSQ.CannonSQ'"));

	SetMaxHP(2);
	SetCurrentHP(2);
}

// ReSharper disable once CppParameterMayBeConst
void AEnemyShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		Location = GetActorLocation();
		Rotation = GetActorRotation();
	}
}

int32 AEnemyShip::GetMaxHP() const
{
	return MaxHP;
}

int32 AEnemyShip::GetCurrentHP() const
{
	return CurrentHP;
}

void AEnemyShip::SetMaxHP(const int32 NewMaxHP)
{
	if (NewMaxHP < 0)
	{
		MaxHP = 0;
		return;
	}
	
	MaxHP = NewMaxHP;
}

void AEnemyShip::SetCurrentHP(const int32 NewCurrentHP)
{
	if (NewCurrentHP < 0)
	{
		CurrentHP = 0;
		return;
	}
	
	CurrentHP = NewCurrentHP;
}

void AEnemyShip::Heal(const int32 HealAmount)
{
	CurrentHP = FMath::Clamp(CurrentHP + HealAmount, 0, MaxHP);
}

void AEnemyShip::Damage(const int32 DamageAmount)
{
	if (CurrentHP == 0)
	{
		return;
	}
	
	CurrentHP = FMath::Clamp(CurrentHP - DamageAmount, 0, MaxHP);

	if (CurrentHP == 0)
	{
		Die();
	}
}

void AEnemyShip::Die()
{
	EnemyShipDieDelegate.Execute(this);
}

void AEnemyShip::MoveToMyShip(const AMyShip* MyShip, const float DeltaTime)
{
	const auto MyShipLocation = MyShip->GetActorLocation();
	const auto MyLocation = GetActorLocation();

	const UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	const UNavigationPath* PathToMyShip = NavSys->FindPathToLocationSynchronously(GetWorld(), MyLocation, MyShipLocation);
	if (!PathToMyShip || PathToMyShip->PathPoints.Num() == 0)
	{
		return;
	}
	
	const FVector NextPoint = PathToMyShip->PathPoints[1];
	const FVector DirectionToNextPoint = NextPoint - GetActorLocation();
	const FVector NewLocation = GetActorLocation() + DirectionToNextPoint.GetSafeNormal() * MoveSpeed * DeltaTime;
	
	SetActorLocation(NewLocation);
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

AEnemy* AEnemyShip::SpawnEnemy(AMyShip* MyShip)
{
	const UEnemySpawnPoint* EnemySpawnPoint = GetEnemySpawnPointToSpawn(MyShip);
	const FVector SpawnLocation = EnemySpawnPoint->GetComponentLocation();
	
	auto SpawnParams = FActorSpawnParameters();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	AEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AEnemy>(AEnemy::StaticClass(), FTransform(UE::Math::TVector<double>(0, 0, 0)), SpawnParams);
	
	SpawnedEnemy->AttachToActor(MyShip, FAttachmentTransformRules::KeepRelativeTransform);
	SpawnedEnemy->SetActorLocation(SpawnLocation);

	CurrentSpawnEnemyCooldown = SpawnEnemyCooldown;
	return SpawnedEnemy;
}

bool AEnemyShip::CanMove(const AMyShip* MyShip) const
{
	const auto MyShipLocation = MyShip->GetActorLocation();
	const auto Direction = MyShipLocation - GetActorLocation();
	return Direction.Size() > DistanceToMyShip;
}

bool AEnemyShip::CanSpawnEnemy(const AMyShip* MyShip) const
{
	if (!bCanSpawnEnemy)
	{
		return false;
	}

	if (CurrentSpawnEnemyCooldown > 0.0f)
	{
		return false;
	}

	const auto MyShipLocation = MyShip->GetActorLocation();
	if (const auto Direction = MyShipLocation - GetActorLocation(); Direction.Size() > DistanceToMyShip)
	{
		return false;
	}

	return true;
}

bool AEnemyShip::CanFireCannon() const
{
	return bCanFireCannon;
}

void AEnemyShip::ReduceSpawnEnemyCooldown(const float DeltaTime)
{
	if (CurrentSpawnEnemyCooldown > 0.0f)
	{
		CurrentSpawnEnemyCooldown -= DeltaTime;
	}
}

UEnemySpawnPoint* AEnemyShip::GetEnemySpawnPointToSpawn(const AMyShip* MyShip) const
{
	// Find two nearest enemy spawn points
	TArray<UEnemySpawnPoint*> EnemySpawnPoints = MyShip->GetEnemySpawnPoints();

	TArray<float> Distances;
	for (const auto EnemySpawnPoint : EnemySpawnPoints)
	{
		const auto Distance = FVector::Dist(EnemySpawnPoint->GetComponentLocation(), GetActorLocation());
		Distances.Add(Distance);
	}

	int FirstNearestIndex = 0;
	int SecondNearestIndex = 0;

	for (int i = 0; i < Distances.Num(); i++)
	{
		if (Distances[i] < Distances[FirstNearestIndex])
		{
			SecondNearestIndex = FirstNearestIndex;
			FirstNearestIndex = i;
		}
		else if (Distances[i] < Distances[SecondNearestIndex])
		{
			SecondNearestIndex = i;
		}
	}

	UEnemySpawnPoint* FirstNearestEnemySpawnPoint = EnemySpawnPoints[FirstNearestIndex];
	UEnemySpawnPoint* SecondNearestEnemySpawnPoint = EnemySpawnPoints[SecondNearestIndex];

	return FMath::RandBool() ? FirstNearestEnemySpawnPoint : SecondNearestEnemySpawnPoint;
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
