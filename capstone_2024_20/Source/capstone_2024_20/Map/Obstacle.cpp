#include "Obstacle.h"

// Todo@autumn - Declare obstacle classes

AObstacle::AObstacle() : MeshComponent(nullptr)
{
	// TODO@autumn - This is a temporary mesh, replace it with the actual mesh from data
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	MeshComponent->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/GameObjects/Terrain/Rock1.Rock1'")));
	RootComponent = MeshComponent;
}
