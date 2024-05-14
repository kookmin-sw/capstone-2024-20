#include "MyShip.h"

#include "GameFramework/PawnMovementComponent.h"
#include "Net/UnrealNetwork.h"

AMyShip::AMyShip()
{
	PrimaryActorTick.bCanEverTick = true;
	M_MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = M_MeshComponent;
}

void AMyShip::BeginPlay()
{
	Super::BeginPlay();

	// Todo@autumn replace with data table
	SetMaxHP(5);
	SetCurrentHP(5);
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
}

void AMyShip::Upgrade()
{
	MoveSpeed += 100.0f; // Todo@autumn replace with data table
}

float AMyShip::GetMoveSpeed() const
{
	return MoveSpeed;
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

void AMyShip::MulticastRPC_SetShipLocation_Implementation(FVector newLoc)
{
	AddActorWorldOffset(newLoc, true);
}

float AMyShip::GetHPPercent()
{

	return (float)GetCurrentHP() / (float)GetMaxHP();
}
