#include "Event.h"
#include "capstone_2024_20/MyShip.h"
#include "capstone_2024_20/WidgetBlueprint/PopupEvent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystemComponent.h"

AEvent::AEvent(): Particle(nullptr), BoxComponent(nullptr)
{
	// TODO@autumn - This is a temporary mesh, replace it with the actual mesh from data
	Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	Particle->SetTemplate(LoadObject<UParticleSystem>(nullptr, TEXT("/Script/Engine.ParticleSystem'/Game/Particles/Realistic_Starter_VFX_Pack_Vol2/Particles/Fire/P_Fire_Big.P_Fire_Big'")));
	Particle->SetRelativeScale3D(AdjustedScale);
	RootComponent = Particle;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	BoxComponent->SetBoxExtent(FVector(500.0f, 500.0f, 300.0f));
	BoxComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	BoxComponent->SetGenerateOverlapEvents(true);

	BoxComponent->ComponentTags.Add(TEXT("Object"));
	BoxComponent->ComponentTags.Add(TEXT("FireEvent"));
	
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/WidgetBlueprints/Event/BP_PopupEvent.BP_PopupEvent_C'"));
    PopupEventWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
    PopupEventWidgetComponent->SetWidgetClass(WidgetClass.Class);
    PopupEventWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    PopupEventWidgetComponent->SetupAttachment(RootComponent);
	
	CanBeOperated = true;
}

void AEvent::BeginPlay()
{
	Super::BeginPlay();
	PopupEvent = Cast<UPopupEvent>(PopupEventWidgetComponent->GetUserWidgetObject());
}

void AEvent::Operate()
{
	EventOperateDelegate.ExecuteIfBound(this);
	Destroy();
}

void AEvent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEvent, CurrentDamageCooldown);
}

void AEvent::DamageMyShip(AMyShip* MyShip)
{
	MyShip->Damage(1);
	CurrentDamageCooldown = DamageCooldown;
}

void AEvent::ReduceCurrentDamageCooldown(const float DeltaTime)
{
	if (CurrentDamageCooldown > 0.0f)
	{
		CurrentDamageCooldown -= DeltaTime;
	}

	MulticastRPC_ReduceCurrentDamageCooldown();
}

void AEvent::MulticastRPC_ReduceCurrentDamageCooldown_Implementation()
{
	PopupEvent->SetDamageCoolDownProgressBarPercent(CurrentDamageCooldown / DamageCooldown);
}

bool AEvent::CanDamageMyShip() const
{
	return CurrentDamageCooldown <= 0.0f;
}
