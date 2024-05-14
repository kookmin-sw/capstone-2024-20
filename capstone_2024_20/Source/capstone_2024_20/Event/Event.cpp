#include "Event.h"

#include "Particles/ParticleSystemComponent.h"

AEvent::AEvent(): Particle(nullptr), BoxComponent(nullptr)
{
	// TODO@autumn - This is a temporary mesh, replace it with the actual mesh from data
	Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	Particle->SetTemplate(LoadObject<UParticleSystem>(nullptr, TEXT("/Script/Engine.ParticleSystem'/Game/Particles/Realistic_Starter_VFX_Pack_Vol2/Particles/Fire/P_Fire_Big.P_Fire_Big'")));
	RootComponent = Particle;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	BoxComponent->SetBoxExtent(FVector(150.0f, 150.0f, 100.0f));
	BoxComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	BoxComponent->SetGenerateOverlapEvents(true);

	BoxComponent->ComponentTags.Add(TEXT("Object"));
	BoxComponent->ComponentTags.Add(TEXT("FireEvent"));

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
