#include "SailingSystem.h"
#include "../EnemyShip/EnemyShip.h"
#include "../Enemy/Enemy.h"
#include "../MyShip.h"
#include "../Event/Event.h"
#include "../Trigger/Trigger.h"
#include "Blueprint/UserWidget.h"
#include "../MyCharacter.h"
#include "Kismet/GameplayStatics.h"

ASailingSystem::ASailingSystem(): ClearTrigger(nullptr), GameOverTrigger(nullptr), MyShip(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASailingSystem::BeginPlay()
{
	Super::BeginPlay();
	
	ClearTrigger = NewObject<UTrigger>();
	ClearTrigger->Initialize("T_0001", this);
	
	GameOverTrigger = NewObject<UTrigger>();
	GameOverTrigger->Initialize("T_0002", this);
	
	// To ensure that the ship is set before sailing system starts, run SetMyShip on world begin play
	GetWorld()->OnWorldBeginPlay.AddUObject(this, &ASailingSystem::SetMyShip);
	GetWorld()->OnWorldBeginPlay.AddUObject(this, &ASailingSystem::SetMyCharacters);
}

// ReSharper disable once CppParameterMayBeConst
void ASailingSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MyShip == nullptr)
	{
		return;
	}

	if (bIsClear)
	{
		return;
	}

	ElapsedTime += DeltaTime;
	
	if (ClearTrigger->IsTriggered() && !bIsClear)
	{
		const auto ClearWidgetRef = TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/WidgetBlueprints/StageClearPopUpWidget.StageClearPopUpWidget_C'");
		if (const auto StagePopUpWidgetClass = StaticLoadClass(UUserWidget::StaticClass(), nullptr,ClearWidgetRef); StagePopUpWidgetClass != nullptr)
		{
			if (UUserWidget* PopUpWidget = CreateWidget<UUserWidget>(GetWorld(), StagePopUpWidgetClass); PopUpWidget != nullptr)
			{
				PopUpWidget->AddToViewport();
			}
		}
		
		bIsClear = true;
	}

	if (GameOverTrigger->IsTriggered())
	{
		// Todo@autumn do something
	}

	SpawnEnemyShipTimer += DeltaTime;

	// Todo@autumn - This is a temporary solution, replace it with data.
	if (SpawnEnemyShipTimer >= 5.0f)
	{
		SpawnEnemyShip();
		SpawnEnemyShipTimer = 0.0f;
	}

	for (const auto EnemyShip : EnemyShips)
	{
		EnemyShip->LookAtMyShip(MyShip);
		EnemyShip->MoveToMyShip(MyShip);

		if (const auto SpawnedEnemy = EnemyShip->SpawnEnemy(MyShip, DeltaTime); SpawnedEnemy != nullptr)
		{
			Enemies.Add(SpawnedEnemy);
		}
	}

	for (const auto Enemy : Enemies)
	{
		// Todo@autumn - This is a temporary solution
		Enemy->MoveToMyCharacter(MyCharacters[0]);
	}

	SpawnEventTimer += DeltaTime;
	// Todo@autumn - This is a temporary solution, replace it with data.
	if (SpawnEventTimer >= 10.0f)
	{
		SpawnEvent();
		SpawnEventTimer = 0.0f;
	}
}

void ASailingSystem::SpawnEnemyShip()
{
	// Todo@autumn - This is a temporary solution, replace it with data.
	auto RandomX = FMath::RandRange(-20000.0f, 20000.0f);
	auto RandomY = FMath::RandRange(-20000.0f, 20000.0f);

	RandomX = RandomX < 0 ? RandomX - 10000.0f : RandomX + 10000.0f;
	RandomY = RandomY < 0 ? RandomY - 10000.0f : RandomY + 10000.0f;
	
	const auto RandomLocation = FVector(RandomX, RandomY, 0.0f);
	AEnemyShip* SpawnedEnemyShip = GetWorld()->SpawnActor<AEnemyShip>(AEnemyShip::StaticClass(), FTransform(RandomLocation));
	EnemyShips.Add(SpawnedEnemyShip);
}

void ASailingSystem::SpawnEvent()
{
	// Todo@autumn - This is a temporary solution, replace it with data.
	const auto RandomX = FMath::RandRange(-100.0f, 100.0f);
	const auto RandomY = FMath::RandRange(-100.0f, 100.0f);
	const auto RandomLocation = FVector(RandomX, RandomY, 850.0f);
	
	AEvent* SpawnedEvent = GetWorld()->SpawnActor<AEvent>(AEvent::StaticClass(), FTransform(UE::Math::TVector<double>(0, 0, 0)));
	SpawnedEvent->AttachToActor(MyShip, FAttachmentTransformRules::KeepRelativeTransform);
	SpawnedEvent->SetActorRelativeLocation(RandomLocation);
	Events.Add(SpawnedEvent);
}

void ASailingSystem::EarnCurrency(const int32 Amount)
{
	// Todo@autumn Need to think maximum currency
	Currency += Amount;
}

void ASailingSystem::UseCurrency(const int32 Amount)
{
	if (Currency < Amount)
	{
		return;
	}
	
	Currency -= Amount;
}

void ASailingSystem::UpgradeMyShip() const
{
	if (MyShip == nullptr)
	{
		return;
	}

	MyShip->Upgrade();
}

float ASailingSystem::GetElapsedTime() const
{
	return ElapsedTime;
}

void ASailingSystem::SetMyShip()
{
	// Todo@autumn - This is a temporary solution, replace it.
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyShip::StaticClass(), FoundActors);
	if (FoundActors.Num() > 0)
	{
		MyShip = Cast<AMyShip>(FoundActors[0]);
	}
}

void ASailingSystem::SetMyCharacters()
{
	TArray<AActor*> FoundMyCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyCharacter::StaticClass(), FoundMyCharacters);
	for (const auto FoundMyCharacter : FoundMyCharacters)
	{
		MyCharacters.Add(Cast<AMyCharacter>(FoundMyCharacter));
	}
}
