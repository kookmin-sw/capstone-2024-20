#include "Enemy.h"

AEnemy::AEnemy(): StaticMesh(nullptr)
{
	// TODO@autumn - This is a temporary mesh, replace it with the actual mesh from data
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/GameObjects/CannonBall/CannonBall_mesh.CannonBall_mesh'")));
	RootComponent = StaticMesh;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}
