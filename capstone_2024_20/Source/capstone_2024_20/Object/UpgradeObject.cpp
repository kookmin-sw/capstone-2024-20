#include "UpgradeObject.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"

AUpgradeObject::AUpgradeObject(): SkeletalMesh(nullptr), BoxComponent(nullptr)
{
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetSkeletalMesh(LoadObject<USkeletalMesh>(nullptr, TEXT("/Script/Engine.SkeletalMesh'/Game/GameObjects/UpgradeNpc/UpgradeNpc.UpgradeNpc'")));
	RootComponent = SkeletalMesh;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	BoxComponent->SetBoxExtent(FVector(150.0f, 150.0f, 100.0f));
	BoxComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	BoxComponent->SetGenerateOverlapEvents(true);

	// Add component tags
	BoxComponent->ComponentTags.Add(TEXT("Object"));
	BoxComponent->ComponentTags.Add(TEXT("Upgrade"));
	
	CanBeOperated = true;
}

void AUpgradeObject::BeginPlay()
{
	Super::BeginPlay();
}

void AUpgradeObject::Operate()
{
	Super::Operate();

	const auto PopupUpgradeRef = TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/WidgetBlueprints/UpgradeObject/BP_PopupUpgrade.BP_PopupUpgrade_C'");
	if (const auto PopupUpgradeClass = StaticLoadClass(UUserWidget::StaticClass(), nullptr,PopupUpgradeRef); PopupUpgradeClass != nullptr)
	{
		if (UUserWidget* PopupUpgrade = CreateWidget<UUserWidget>(GetWorld(), PopupUpgradeClass); PopupUpgrade != nullptr)
		{
			PopupUpgrade->AddToViewport();

			// enable mouse cursor
			GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
		}
	}	
}
