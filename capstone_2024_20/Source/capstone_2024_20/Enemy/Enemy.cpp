#include "Enemy.h"
#include "../MyCharacter.h"

AEnemy::AEnemy(): StaticMesh(nullptr)
{
	// TODO@autumn - This is a temporary mesh, replace it with the actual mesh from data
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/GameObjects/TreasureBox/TreasureBox_Box.TreasureBox_Box'")));
	RootComponent = StaticMesh;

	// 적이 해적을 향해 이동 시 물리적 충돌을 발생시키지 않도록 하기 위함
	StaticMesh->SetSimulatePhysics(false);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));

	// Todo@autumn - This is a temporary solution, replace it with data.
	SetMaxHP(2);
	SetCurrentHP(2);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemy::Die()
{
	IHP::Die();
	EnemyDieDelegate.Execute(this);
}

void AEnemy::MoveToMyCharacter(const AMyCharacter* MyCharacter)
{
	const auto MyCharacterLocation = MyCharacter->GetActorLocation();
	const auto Direction = MyCharacterLocation - GetActorLocation();

	if (Direction.Size() < 100.0f) // Todo@autumn - This is a temporary solution, replace it with data.
	{
		return;
	}

	const auto NormalizedDirection = Direction.GetSafeNormal();
	constexpr auto Speed = 100.0f; // Todo@autumn - This is a temporary solution, replace it with data.
	const auto NewLocation = GetActorLocation() + NormalizedDirection * Speed * GetWorld()->DeltaTimeSeconds;
	SetActorLocation(NewLocation);
}
