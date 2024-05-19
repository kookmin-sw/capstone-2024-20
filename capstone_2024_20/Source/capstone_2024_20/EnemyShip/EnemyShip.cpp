#include "EnemyShip.h"
#include "EnemyShipHead.h"
#include "NavigationPath.h"
#include "../MyShip.h"
#include "../Enemy/Enemy.h"
#include "NavigationSystem.h"
#include "capstone_2024_20/MyIngameHUD.h"
#include "capstone_2024_20/Enemy/EnemySpawnPoint.h"
#include "Particles/ParticleSystem.h"
#include "capstone_2024_20/Object/EnemyShipCannonBall.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AEnemyShip::AEnemyShip()
{
	AActor::SetReplicateMovement(true);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->InitSphereRadius(50.0f);
	SphereComponent->SetCollisionProfileName(TEXT("OverlapAll"));
	SphereComponent->SetGenerateOverlapEvents(true);
	SphereComponent->SetupAttachment(RootComponent);
}

void AEnemyShip::BeginPlay()
{
	Super::BeginPlay();

	ProjectileSpawnPoint = FindComponentByClass<UArrowComponent>();
	ProjectileClass = AEnemyShipCannonBall::StaticClass();
	FireEffect = LoadObject<UParticleSystem>(nullptr, TEXT("/Script/Engine.ParticleSystem'/Game/Particles/Realistic_Starter_VFX_Pack_Vol2/Particles/Explosion/P_Explosion_Big_A.P_Explosion_Big_A'"));
	CannonSoundCue = LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/Sounds/Cannon/CannonSQ.CannonSQ'"));
	MyInGameHUD = Cast<AMyIngameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyShip::BeginOverlap);
	
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

void AEnemyShip::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEnemyShip, MaxHP);
	DOREPLIFETIME(AEnemyShip, CurrentHP);
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
	MulticastRPC_Damage();
	
	if (CurrentHP == 0)
	{
		Die();
	}
}

void AEnemyShip::Die()
{
	MulticastRPC_Die();
	EnemyShipDieDelegate.Execute(this);
}

void AEnemyShip::MulticastRPC_Damage_Implementation()
{
	MyInGameHUD->SetEnemyShipHPProgressBarPercent(static_cast<float>(CurrentHP) / MaxHP);
}

void AEnemyShip::MulticastRPC_Die_Implementation()
{
	MyInGameHUD->SetEnemyShipHPProgressBarVisibility(false);
}

void AEnemyShip::MoveToMyShip(const AMyShip* MyShip, const float DeltaTime)
{
	const FVector EnemyShipHeadLocation = FindComponentByClass<UEnemyShipHead>()->GetComponentLocation();
	const FVector NearestMovePointLocation = MyShip->GetNearestEnemyShipMovePointLocationFrom(EnemyShipHeadLocation);
	const UNavigationPath* PathToMyShip = NavSys->FindPathToLocationSynchronously(GetWorld(), EnemyShipHeadLocation, NearestMovePointLocation);
	
	if (!PathToMyShip || !PathToMyShip->IsValid() || PathToMyShip->PathPoints.Num() < 2)
	{
		return;
	}
	
	const FVector NextPoint = PathToMyShip->PathPoints[1];
	const FVector DirectionToNextPoint = NextPoint - GetActorLocation();
	const FVector NewLocation = GetActorLocation() + DirectionToNextPoint.GetSafeNormal() * MoveSpeed * DeltaTime;
	
	SetActorLocation(NewLocation);
	SetActorRotation(DirectionToNextPoint.Rotation());
}

bool AEnemyShip::CanMove(const AMyShip* MyShip) const
{
	const auto MyShipLocation = MyShip->GetActorLocation();
	const auto Direction = MyShipLocation - GetActorLocation();
	return Direction.Size() > DistanceToMyShip;
}

bool AEnemyShip::CanSpawnEnemy(const AMyShip* MyShip) const
{
	return bCanSpawnEnemy;
}

bool AEnemyShip::CanFireCannon() const
{
	return bCanFireCannon;
}

TArray<UEnemySpawnPoint*> AEnemyShip::GetEnemySpawnPointsToSpawn(const AMyShip* MyShip) const
{
	// Find three nearest enemy spawn points
	TArray<UEnemySpawnPoint*> EnemySpawnPoints = MyShip->GetEnemySpawnPoints();

	TArray<float> Distances;
	for (const auto EnemySpawnPoint : EnemySpawnPoints)
	{
		const auto Distance = FVector::Dist(EnemySpawnPoint->GetComponentLocation(), GetActorLocation());
		Distances.Add(Distance);
	}

	int FirstNearestIndex = 0;
	int SecondNearestIndex = 0;
	int ThirdNearestIndex = 0;

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
		else if (Distances[i] < Distances[ThirdNearestIndex])
		{
			ThirdNearestIndex = i;
		}
	}

	UEnemySpawnPoint* FirstNearestEnemySpawnPoint = EnemySpawnPoints[FirstNearestIndex];
	UEnemySpawnPoint* SecondNearestEnemySpawnPoint = EnemySpawnPoints[SecondNearestIndex];
	UEnemySpawnPoint* ThirdNearestEnemySpawnPoint = EnemySpawnPoints[ThirdNearestIndex];

	TArray<UEnemySpawnPoint*> NearestEnemySpawnPoints;
	NearestEnemySpawnPoints.Add(FirstNearestEnemySpawnPoint);
	NearestEnemySpawnPoints.Add(SecondNearestEnemySpawnPoint);
	NearestEnemySpawnPoints.Add(ThirdNearestEnemySpawnPoint);

	return NearestEnemySpawnPoints;
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

void AEnemyShip::SpawnEnemies(AMyShip* MyShip) const
{
	const TArray<UEnemySpawnPoint*> EnemySpawnPoints = GetEnemySpawnPointsToSpawn(MyShip);
	TArray<FVector> SpawnLocations;

	for (const auto EnemySpawnPoint : EnemySpawnPoints)
	{
		SpawnLocations.Add(EnemySpawnPoint->GetComponentLocation());
	}

	TArray<AEnemy*> SpawnedEnemies;
	auto SpawnParams = FActorSpawnParameters();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	for (const auto SpawnLocation : SpawnLocations)
	{
		AEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AEnemy>(AEnemy::StaticClass(), FTransform(UE::Math::TVector<double>(0, 0, 0)), SpawnParams);
		SpawnedEnemy->AttachToActor(MyShip, FAttachmentTransformRules::KeepRelativeTransform);
		SpawnedEnemy->SetActorLocation(SpawnLocation);

		SpawnedEnemies.Add(SpawnedEnemy);
	}
	
	SpawnEnemyDelegate.Execute(SpawnedEnemies);
}

// For binding to the OnComponentBeginOverlap event
// ReSharper disable once CppMemberFunctionMayBeConst
void AEnemyShip::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority())
	{
		return;
	}

	AMyShip* MyShip = Cast<AMyShip>(OtherActor);
	if (!MyShip)
	{
		return;
	}
 
	if (!CanSpawnEnemy(MyShip))
	{
		return;
	}

	SpawnEnemies(MyShip);
	Die();
}
