#include "ReplicatedCharacter.h"
#include "Net/UnrealNetwork.h"

AReplicatedCharacter::AReplicatedCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootComponent);
}

void AReplicatedCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AReplicatedCharacter, Location);
	DOREPLIFETIME(AReplicatedCharacter, Rotation);
}

void AReplicatedCharacter::OnRep_Location()
{
	if (HasAuthority())
	{
		Location = GetActorLocation();
	}
	else
	{
		SetActorLocation(Location);
	}
}

void AReplicatedCharacter::OnRep_Rotation()
{
	if (HasAuthority())
	{
		Rotation = GetActorRotation();
	}
	else
	{
		SetActorRotation(Rotation);
	}
}

void AReplicatedCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AReplicatedCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
