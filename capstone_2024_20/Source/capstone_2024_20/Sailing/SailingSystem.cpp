#include "SailingSystem.h"
#include "../EnemyShip/EnemyShip.h"
#include "../Enemy/Enemy.h"
#include "../MyShip.h"
#include "../Event/Event.h"
#include "../Trigger/Trigger.h"
#include "../Map/Obstacle.h"
#include "Blueprint/UserWidget.h"
#include "../MyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "../Map/Map.h"
#include "../Map/Grid.h"
#include "../Object/UpgradeObject.h"

ASailingSystem::ASailingSystem(): Map(nullptr), ClearTrigger(nullptr), GameOverTrigger(nullptr), MyShip(nullptr)
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
	
	CreateMap();

	// Todo@autumn - This is a temporary solution, replace it with data.
	
	//Spawn UpgradeObject
	const FVector Location = FVector(1400, 400, 1000);
	AUpgradeObject* SpawnedUpgradeObject = GetWorld()->SpawnActor<AUpgradeObject>(AUpgradeObject::StaticClass(), FTransform(UE::Math::TVector<double>(0, 0, 0)));
	SpawnedUpgradeObject->AttachToActor(MyShip, FAttachmentTransformRules::KeepRelativeTransform);
	SpawnedUpgradeObject->SetActorRelativeLocation(Location);
	
	// To ensure that the ship is set before sailing system starts, run SetMyShip on world begin play
	GetWorld()->OnWorldBeginPlay.AddUObject(this, &ASailingSystem::SetMyShip);
	GetWorld()->OnWorldBeginPlay.AddUObject(this, &ASailingSystem::SetMyCharacters);
	GetWorld()->OnWorldBeginPlay.AddUObject(this, &ASailingSystem::SetEnemyShips);
}

// ReSharper disable once CppParameterMayBeConst
void ASailingSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority())
	{
		return;
	}

	if (MyShip == nullptr)
	{
		return;
	}

	if (bIsClear || bIsGameOver)
	{
		return;
	}

	ElapsedTime += DeltaTime;
	
	SetMyCharacters();
	
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
		const auto GameOverWidgetRef = TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/WidgetBlueprints/StageFailPopUpWidget.StageFailPopUpWidget_C'");
		if (const auto StagePopUpWidgetClass = StaticLoadClass(UUserWidget::StaticClass(), nullptr,GameOverWidgetRef); StagePopUpWidgetClass != nullptr)
		{
			if (UUserWidget* PopUpWidget = CreateWidget<UUserWidget>(GetWorld(), StagePopUpWidgetClass); PopUpWidget != nullptr)
			{
				PopUpWidget->AddToViewport();
			}
		}
		
		bIsGameOver = true;
	}

	for (const auto EnemyShip : EnemyShips)
	{
		EnemyShip->LookAtMyShip(MyShip);
		EnemyShip->FireCannon(DeltaTime);

		if (EnemyShip->CanSpawnEnemy())
		{
			if (const auto SpawnedEnemy = EnemyShip->SpawnEnemy(MyShip, DeltaTime); SpawnedEnemy != nullptr)
			{
				Enemies.Add(SpawnedEnemy);
				SpawnedEnemy->EnemyDieDelegate.BindUObject(this, &ASailingSystem::OnEnemyDie);
			}
		}
	}

	for (const auto Enemy : Enemies)
	{
		AMyCharacter* NearestMyCharacter = FindNearestMyCharacter(Enemy);

		if (Enemy->CanMove())
		{
			Enemy->MoveToMyCharacter(NearestMyCharacter);
		}

		if (Enemy->CanAttack())
		{
			if (const float Distance = FVector::Dist(Enemy->GetActorLocation(), NearestMyCharacter->GetActorLocation()); Distance <= Enemy->GetDistanceToMyCharacter())
			{
				Enemy->Attack(NearestMyCharacter);
			}	
		}

		Enemy->ReduceCurrentAttackCooldown(DeltaTime);
	}

	CalculateEnemyInAttackRange();

	SpawnEventTimer += DeltaTime;
	// Todo@autumn - This is a temporary solution, replace it with data.
	if (SpawnEventTimer >= 10.0f)
	{
		// SpawnEvent();
		SpawnEventTimer = 0.0f;
	}
}

void ASailingSystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ASailingSystem::OnEnemyDie(AEnemy* Enemy)
{
	Enemies.Remove(Enemy);
	Enemy->Destroy();
	EarnCurrency(100); // Todo@autumn - This is a temporary solution, replace it with data.
}

void ASailingSystem::OnEnemyShipDie(AEnemyShip* EnemyShip)
{
	EnemyShips.Remove(EnemyShip);
	EnemyShip->Destroy();
	EarnCurrency(100);
}

void ASailingSystem::CreateMap()
{
	Map = NewObject<UMap>();
	Map->Initialize();
	Map->CellularAutomata();

	// ! 기술 설명 등 필요한 경우에 사용할 수 있도록, 제거하지 않고 주석 처리하여 남겨둔다.
	// CreateObstacles();
}

void ASailingSystem::CreateObstacles() const
{
	for (auto ObstacleGrids = Map->GetObstacleGrids(); const auto ObstacleGrid : ObstacleGrids)
	{
		const auto SpawnedObstacle = GetWorld()->SpawnActor<AObstacle>(AObstacle::StaticClass(), ObstacleGrid->GetTransform());
		SpawnedObstacle->SetActorRotation(ObstacleGrid->GetRotator());
	}
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

void ASailingSystem::CalculateEnemyInAttackRange()
{
	for (const auto Character : MyCharacters)
	{
		FTransform CharacterTransform = Character->GetActorTransform();

		double MinDistance = TNumericLimits<double>::Max();
		AEnemy* EnemyInAttackRange = nullptr;
		
		for (const auto Enemy : Enemies)
		{
			// Todo@autumn - This is a temporary solution, replace it with data.
			if (const auto Distance = FVector::Dist(CharacterTransform.GetLocation(), Enemy->GetActorLocation()); Distance < 200.0f && Distance < MinDistance)
			{
				MinDistance = Distance;
				EnemyInAttackRange = Enemy;
			}
		}

		// ! nullable
		Character->SetEnemyInAttackRange(EnemyInAttackRange);
	}
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

AMyShip* ASailingSystem::GetMyShip() const
{
	return MyShip;
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
	MyCharacters.Empty();
	
	TArray<AActor*> FoundMyCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyCharacter::StaticClass(), FoundMyCharacters);
	for (const auto FoundMyCharacter : FoundMyCharacters)
	{
		MyCharacters.Add(Cast<AMyCharacter>(FoundMyCharacter));
	}
}

void ASailingSystem::SetEnemyShips()
{
	EnemyShips.Empty();

	TArray<AActor*> FoundEnemyShips;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyShip::StaticClass(), FoundEnemyShips);
	for (const auto FoundEnemyShip : FoundEnemyShips)
	{
		EnemyShips.Add(Cast<AEnemyShip>(FoundEnemyShip));
	}
}

AMyCharacter* ASailingSystem::FindNearestMyCharacter(const AEnemy* Enemy) const
{
	AMyCharacter* NearestMyCharacter = MyCharacters[0];

	for (const auto MyCharacter : MyCharacters)
	{
		if (const auto Distance = FVector::Dist(Enemy->GetActorLocation(), MyCharacter->GetActorLocation()); Distance < FVector::Dist(Enemy->GetActorLocation(), NearestMyCharacter->GetActorLocation()))
		{
			NearestMyCharacter = MyCharacter;
		}
	}

	return NearestMyCharacter;
}
