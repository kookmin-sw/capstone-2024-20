#include "Event.h"

AEvent::AEvent(): StaticMesh(nullptr), BoxComponent(nullptr)
{
	// TODO@autumn - This is a temporary mesh, replace it with the actual mesh from data
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/GameObjects/Wood/wood_mesh.wood_mesh'")));
	RootComponent = StaticMesh;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	BoxComponent->SetBoxExtent(FVector(150.0f, 150.0f, 100.0f));
	BoxComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	BoxComponent->SetGenerateOverlapEvents(true);

	CanBeOperated = true;
}

void AEvent::BeginPlay()
{
	Super::BeginPlay();
}

void AEvent::Operate()
{
	// Todo@autumn - This is a temporary implementation, need to replace it.
	Destroy();
}
