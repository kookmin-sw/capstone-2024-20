#include "MyShip.h"
#include "Net/UnrealNetwork.h"
#include "MyCannon.h"
#include "Components/BoxComponent.h"
#include "Enemy/EnemySpawnPoint.h"
#include "EnemyShip/EnemyShipMovePoint.h"
#include "Event/EventSpawnPoint.h"
#include "Kismet/GameplayStatics.h"

AMyShip::AMyShip()
{
	PrimaryActorTick.bCanEverTick = true;
	M_MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = M_MeshComponent;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetCollisionProfileName(TEXT("OverlapAll"));
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->SetBoxExtent(FVector(3000.0f, 1000.0f, 1000.0f));
}

void AMyShip::BeginPlay()
{
	Super::BeginPlay();

	// Todo@autumn replace with data table
	SetMaxHP(5);
	SetCurrentHP(5);
	FindMyCannons();
	FindEnemySpawnPoints();
	FindEventSpawnPoints();
	FindEnemyShipMovePoints();
}

// ReSharper disable once CppParameterMayBeConst
void AMyShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorRotation(TargetRotation);
}

void AMyShip::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyShip, TargetRotation);
	DOREPLIFETIME(AMyShip, MaxHP);
	DOREPLIFETIME(AMyShip, CurrentHP);
}

void AMyShip::UpgradeMoveSpeed()
{
	MoveSpeed += 100.0f;
}

void AMyShip::UpgradeHandling()
{
	RotationAcceleration += 1.0f;
}

void AMyShip::UpgradeCannonAttack()
{
	for (AMyCannon* Cannon : MyCannons)
	{
		Cannon->UpgradeAttackDamage();
	}
}

float AMyShip::GetMoveSpeed() const
{
	return MoveSpeed;
}

float AMyShip::GetRotationAcceleration() const
{
	return RotationAcceleration;
}

FVector AMyShip::GetNearestEventSpawnPointLocationFrom(const FVector& FromLocation) const
{
	FVector NearestLocation = EnemySpawnPoints[0]->GetComponentLocation();
	float NearestDistance = FVector::Dist(FromLocation, NearestLocation);

	for (const UEventSpawnPoint* EventSpawnPoint : EventSpawnPoints)
	{
		if (FVector::Dist(FromLocation, EventSpawnPoint->GetComponentLocation()) < NearestDistance)
		{
			NearestLocation = EventSpawnPoint->GetComponentLocation();
			NearestDistance = FVector::Dist(FromLocation, NearestLocation);
		}
	}

	return NearestLocation;
}

FVector AMyShip::GetNearestEnemyShipMovePointLocationFrom(const FVector& FromLocation) const
{
	FVector NearestLocation = EnemyShipMovePoints[0]->GetComponentLocation();
	float NearestDistance = FVector::Dist(FromLocation, NearestLocation);

	for (const UEnemyShipMovePoint* EnemyShipMovePoint : EnemyShipMovePoints)
	{
		if (FVector::Dist(FromLocation, EnemyShipMovePoint->GetComponentLocation()) < NearestDistance)
		{
			NearestLocation = EnemyShipMovePoint->GetComponentLocation();
			NearestDistance = FVector::Dist(FromLocation, NearestLocation);
		}
	}

	return NearestLocation;
}

void AMyShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

int32 AMyShip::GetMaxHP() const
{
	return MaxHP;
}

int32 AMyShip::GetCurrentHP() const
{
	return CurrentHP;
}

void AMyShip::SetMaxHP(const int32 NewMaxHP)
{
	if (NewMaxHP < 0)
	{
		MaxHP = 0;
		return;
	}
	
	MaxHP = NewMaxHP;
}

void AMyShip::SetCurrentHP(const int32 NewCurrentHP)
{
	if (NewCurrentHP < 0)
	{
		CurrentHP = 0;
		return;
	}
	
	CurrentHP = NewCurrentHP;
}

void AMyShip::Heal(const int32 HealAmount)
{
	CurrentHP = FMath::Clamp(CurrentHP + HealAmount, 0, MaxHP);
}

void AMyShip::Damage(const int32 DamageAmount)
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

void AMyShip::Die()
{
	// do nothing
}

void AMyShip::MulticastRPC_SetShipLocation_Implementation(const FVector NewLoc)
{
	AddActorWorldOffset(NewLoc, true);
}

float AMyShip::GetHPPercent() const
{
	return static_cast<float>(GetCurrentHP()) / static_cast<float>(GetMaxHP());
}

TArray<UEnemySpawnPoint*> AMyShip::GetEnemySpawnPoints() const
{
	return EnemySpawnPoints;
}

void AMyShip::FindMyCannons()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyCannon::StaticClass(), FoundActors);
	if (FoundActors.Num() > 0)
	{
		for (AActor* Actor : FoundActors)
		{
			if (AMyCannon* Cannon = Cast<AMyCannon>(Actor))
			{
				MyCannons.Add(Cannon);
			}
		}
	}
}

void AMyShip::FindEnemySpawnPoints()
{
	for (TSet<UActorComponent*> Components = GetComponents(); UActorComponent*& Component : Components)
	{
		if (UEnemySpawnPoint* EnemySpawnPoint = Cast<UEnemySpawnPoint>(Component))
		{
			EnemySpawnPoints.Add(EnemySpawnPoint);
		}
	}
}

void AMyShip::FindEventSpawnPoints()
{
	for (TSet<UActorComponent*> Components = GetComponents(); UActorComponent*& Component : Components)
	{
		if (UEventSpawnPoint* EventSpawnPoint = Cast<UEventSpawnPoint>(Component))
		{
			EventSpawnPoints.Add(EventSpawnPoint);
		}
	}
}

void AMyShip::FindEnemyShipMovePoints()
{
	for (TSet<UActorComponent*> Components = GetComponents(); UActorComponent*& Component : Components)
	{
		if (UEnemyShipMovePoint* EnemyShipMovePoint = Cast<UEnemyShipMovePoint>(Component))
		{
			EnemyShipMovePoints.Add(EnemyShipMovePoint);
		}
	}
}
