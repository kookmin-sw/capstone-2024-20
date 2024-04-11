#include "Obstacle.h"

AObstacle::AObstacle() : MeshComponent(nullptr)
{
	// TODO@autumn - This is a temporary mesh, replace it with the actual mesh from data
	
	const auto Random = FMath::RandRange(1, 3);
	const auto Name = FString::Printf(TEXT("/Game/GameObjects/Terrain/Rock%d.Rock%d"), Random, Random);
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	MeshComponent->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, *Name));
	RootComponent = MeshComponent;
}
