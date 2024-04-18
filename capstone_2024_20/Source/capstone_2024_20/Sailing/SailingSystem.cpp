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

	GenerateMap();
	
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

void ASailingSystem::GenerateMap()
{
	InitMap();
	DivideMap();
	CreateObstacles();
}

void ASailingSystem::InitMap()
{
	constexpr int32 GridCount = 20; // Todo@autumn - This is a temporary solution, replace it with data.

	Map.Init(TArray<int>(), GridCount);
	for (int i = 0; i < GridCount; i++)
	{
		Map[i].Init(0, GridCount);
	}
}

void ASailingSystem::DivideMap()
{
	int Width = Map.Num();
	int Height = Map[0].Num();

	TQueue<int> LeftBoundQueue;
	TQueue<int> RightBoundQueue;
	TQueue<int> TopBoundQueue;
	TQueue<int> BottomBoundQueue;

	LeftBoundQueue.Enqueue(-1);
	RightBoundQueue.Enqueue(Width);
	TopBoundQueue.Enqueue(-1);
	BottomBoundQueue.Enqueue(Height);
	
	for (int i = 0; i < 10; i++)
	{
		constexpr int MinHeight = 5;
		int LeftBound;
		int RightBound;
		int TopBound;
		int BottomBound;

		LeftBoundQueue.Dequeue(LeftBound);
		RightBoundQueue.Dequeue(RightBound);
		TopBoundQueue.Dequeue(TopBound);
		BottomBoundQueue.Dequeue(BottomBound);
		
		Width = RightBound - LeftBound - 1;
		Height = BottomBound - TopBound - 1;

		if (constexpr int MinWidth = 5; Width < MinWidth || Height < MinHeight)
		{
			continue;
		}
		
		if (Width < Height)
		{
			// Divide horizontally
			
			const int Mid = FMath::RandRange(TopBound + Height * 0.4, BottomBound - Height * 0.4);
			
			for (int x = LeftBound + 1; x < RightBound; x++)
			{
				Map[x][Mid] = 1;
			}

			LeftBoundQueue.Enqueue(LeftBound);
			RightBoundQueue.Enqueue(RightBound);
			TopBoundQueue.Enqueue(TopBound);
			BottomBoundQueue.Enqueue(Mid);

			LeftBoundQueue.Enqueue(LeftBound);
			RightBoundQueue.Enqueue(RightBound);
			TopBoundQueue.Enqueue(Mid);
			BottomBoundQueue.Enqueue(BottomBound);
		}
		else
		{
			// Divide vertically
			
			const int Mid = FMath::RandRange(LeftBound + Width * 0.4, RightBound - Width * 0.4);
			
			for (int y = TopBound + 1; y < BottomBound; y++)
			{
				Map[Mid][y] = 1;
			}

			LeftBoundQueue.Enqueue(LeftBound);
			RightBoundQueue.Enqueue(Mid);
			TopBoundQueue.Enqueue(TopBound);
			BottomBoundQueue.Enqueue(BottomBound);

			LeftBoundQueue.Enqueue(Mid);
			RightBoundQueue.Enqueue(RightBound);
			TopBoundQueue.Enqueue(TopBound);
			BottomBoundQueue.Enqueue(BottomBound);
		}
	}

	LeftBoundQueue.Empty();
	RightBoundQueue.Empty();
	TopBoundQueue.Empty();
	BottomBoundQueue.Empty();
}

void ASailingSystem::CreateObstacles()
{
	for(int x = 0; x < Map.Num(); x++)
	{
		for (int y = 0; y < Map[x].Num(); y++)
		{
			switch (Map[x][y])
			{
			case 0:
				continue;
			case 1:
				CreateObstacle(x, y);
				break;
			default:
				continue;
			}
		}
	}
}

void ASailingSystem::CreateObstacle(const int32 Row, const int32 Col)
{
	constexpr float GridSize = 10000.0f;
	const float XPos = (Col - Map.Num() / 2) * GridSize + GridSize / 2;
	const float YPos = (Row - Map[Col].Num() / 2) * GridSize + GridSize / 2;
	const FTransform GridTransform = FTransform(FVector(XPos, YPos, 0.0f));
	const FRotator RandRotator = FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f);

	const auto SpawnedObstacle = GetWorld()->SpawnActor<AObstacle>(AObstacle::StaticClass(), GridTransform);
	SpawnedObstacle->SetActorRotation(RandRotator);
}

// For debugging. Do not remove even if it is not used.
void ASailingSystem::DebugMap()
{
	for (int y = 0; y < Map.Num(); y++)
	{
		FString RowString;
		for (int x = 0; x < Map[0].Num(); x++)
		{
			RowString += FString::FromInt(Map[x][y]);
		}
		UE_LOG(LogTemp, Warning, TEXT("%s"), *RowString);
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
