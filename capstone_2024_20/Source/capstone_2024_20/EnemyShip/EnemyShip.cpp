#include "EnemyShip.h"
#include "../MyShip.h"
#include "../Enemy/Enemy.h"

AEnemyShip::AEnemyShip(): StaticMesh(nullptr)
{
	// TODO@autumn - This is a temporary mesh, replace it with the actual mesh from data
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/GameObjects/FlatShip/Ship_Ship.Ship_Ship'")));
	RootComponent = StaticMesh;

	SetMaxHP(2);
	SetCurrentHP(2);
}

void AEnemyShip::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyShip::LookAtMyShip(const AMyShip* MyShip)
{
	const auto Direction = MyShip->GetActorLocation() - GetActorLocation();
	const auto Rotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	SetActorRotation(Rotation);
}

void AEnemyShip::MoveToMyShip(const AMyShip* MyShip)
{
	const auto MyShipLocation = MyShip->GetActorLocation();
	const auto Direction = MyShipLocation - GetActorLocation();

	// Todo@autumn - This is a temporary solution, replace it with data.
	if (Direction.Size() < 11000.0f)
	{
		return;
	}
	
	const auto NormalizedDirection = Direction.GetSafeNormal();
	constexpr auto Speed = 2500.0f; // Todo@autumn - This is a temporary solution, replace it with data.
	const auto NewLocation = GetActorLocation() + NormalizedDirection * Speed * GetWorld()->DeltaTimeSeconds;
	SetActorLocation(NewLocation);
}

AEnemy* AEnemyShip::SpawnEnemy(AActor* MyShip, const float DeltaTime) const
{
	SpawnEnemyTimer += DeltaTime;

	// Todo@autumn - This is a temporary solution, replace it with data.
	if (SpawnEnemyTimer < 5.0f) 
	{
		return nullptr;
	}

	const auto MyShipLocation = MyShip->GetActorLocation();
	if (const auto Direction = MyShipLocation - GetActorLocation(); Direction.Size() > 12000.0f) // Todo@autumn - This is a temporary solution, replace it with data.
	{
		return nullptr;
	}
	
	// Todo@autumn - This is a temporary solution, replace it with data.
	const auto RandomX = FMath::RandRange(-100.0f, 100.0f);
	const auto RandomY = FMath::RandRange(-100.0f, 100.0f);
	const auto RandomLocation = FVector(RandomX, RandomY, 880.0f);

	AEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AEnemy>(AEnemy::StaticClass(), FTransform(UE::Math::TVector<double>(0, 0, 0)));
	SpawnedEnemy->AttachToActor(MyShip, FAttachmentTransformRules::KeepRelativeTransform);
	SpawnedEnemy->SetActorRelativeLocation(RandomLocation);

	SpawnEnemyTimer = 0.0f;

	return SpawnedEnemy;
}

void AEnemyShip::Die()
{
	IHP::Die();
	EnemyShipDieDelegate.Execute(this);
}

