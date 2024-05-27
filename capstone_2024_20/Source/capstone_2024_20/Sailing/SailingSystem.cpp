#include "SailingSystem.h"
#include "../EnemyShip/EnemyShip.h"
#include "../Enemy/Enemy.h"
#include "../MyShip.h"
#include "../Trigger/Trigger.h"
#include "../Map/Obstacle.h"
#include "Blueprint/UserWidget.h"
#include "../MyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "../Map/Map.h"
#include "../Map/Grid.h"
#include "../Object/Destination.h"
#include "Net/UnrealNetwork.h"
#include "../Event/Event.h"
#include "capstone_2024_20/MyIngameHUD.h"

ASailingSystem::ASailingSystem(): Map(nullptr), ClearTrigger(nullptr), GameOverTrigger(nullptr), MyShip(nullptr),
                                  Destination(nullptr)
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

	// To ensure that the ship is set before sailing system starts, run SetMyShip on world begin play
	GetWorld()->OnWorldBeginPlay.AddUObject(this, &ASailingSystem::SetMyShip);
	GetWorld()->OnWorldBeginPlay.AddUObject(this, &ASailingSystem::SetMyCharacters);
	GetWorld()->OnWorldBeginPlay.AddUObject(this, &ASailingSystem::SetEnemyShips);
	GetWorld()->OnWorldBeginPlay.AddUObject(this, &ASailingSystem::SetDestination);
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

	for (const auto MyCharacter : MyCharacters)
	{
		if (MyCharacter->GetCurrentPlayerState() == UserState::DEAD)
		{
			MyCharacter->ReduceReviveCooldown(DeltaTime);

			if (MyCharacter->CanRevive())
			{
				MyCharacter->Revive();
			}
		}
		
		MyCharacter->ReduceAttackCooldown(DeltaTime);
	}

	for (const auto EnemyShip : EnemyShips)
	{
		if (EnemyShip->CanMove(MyShip))
		{
			EnemyShip->MoveToMyShip(MyShip, DeltaTime);

			if (bIsEnemyShipFirstMove)
			{
				MulticastRPC_Caution(FText::FromString(FString::Printf(TEXT("적에게 발각됐습니다!"))));
				bIsEnemyShipFirstMove = false;
			}
		}

		if (EnemyShip->CanFireCannon(MyShip))
		{
			EnemyShip->FireCannon(DeltaTime);

			if (bIsEnemyShipFirstFire)
			{
				MulticastRPC_Caution(FText::FromString(FString::Printf(TEXT("적 함선이 포탄을 발사합니다!"))));
				bIsEnemyShipFirstFire = false;
			}
		}
	}

	for (const auto Enemy : Enemies)
	{
		if (const AMyCharacter* NearestMyCharacter = FindNearestMyCharacter(Enemy); NearestMyCharacter != nullptr)
		{
			if (Enemy->CanMove())
			{
				Enemy->MoveToMyCharacter(NearestMyCharacter);
			}

			if (Enemy->CanAttack())
			{
				if (const float Distance = FVector::Dist(Enemy->GetActorLocation(), NearestMyCharacter->GetActorLocation()); Distance <= Enemy->GetDistanceToMyCharacter())
				{
					Enemy->Attack();
				}	
			}	
		}

		Enemy->ReduceCurrentAttackCooldown(DeltaTime);
	}

	for (const auto Event : Events)
	{
		if (Event->CanDamageMyShip())
		{
			Event->DamageMyShip(MyShip);
		}
		
		Event->ReduceCurrentDamageCooldown(DeltaTime);
	}
}

void ASailingSystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASailingSystem, Currency);
	//DOREPLIFETIME(ASailingSystem, Progress);
}

void ASailingSystem::OnEnemyDie(AEnemy* Enemy)
{
	Enemies.Remove(Enemy);
	Enemy->Destroy();
	EarnCurrency(100); // Todo@autumn - This is a temporary solution, replace it with data.
}

void ASailingSystem::OnEnemiesSpawned(TArray<AEnemy*> SpawnedEnemies)
{
	for (const auto SpawnedEnemy : SpawnedEnemies)
	{
		SpawnedEnemy->EnemyDieDelegate.BindUObject(this, &ASailingSystem::OnEnemyDie);
		Enemies.Add(SpawnedEnemy);
	}

	if (bIsEnemyShipFirstMove)
	{
		MulticastRPC_Caution(FText::FromString(FString::Printf(TEXT("적이 침입했습니다!"))));
		bIsEnemyShipFirstMove = false;
	}
}

void ASailingSystem::OnEnemyShipDie(AEnemyShip* EnemyShip)
{
	if (EnemyShip->bCanFireCannon)
	{
		MulticastRPC_Caution(FText::FromString(FString::Printf(TEXT("적 함선을 격침했습니다!"))));
	}
	
	EnemyShips.Remove(EnemyShip);
	EnemyShip->Destroy();
	EarnCurrency(100);
}

void ASailingSystem::OnEventOperate(AEvent* Event)
{
	Events.Remove(Event);
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

void ASailingSystem::EarnCurrency(const int32 Amount)
{
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

int ASailingSystem::GetCurrency() const
{
	return Currency;
}

void ASailingSystem::AddSpawnedEventFromEnemyShipCannonBall(AEvent* Event)
{
	Event->EventOperateDelegate.BindUObject(this, &ASailingSystem::OnEventOperate);
	Events.Add(Event);
	MulticastRPC_Caution(FText::FromString(FString::Printf(TEXT("화재가 발생했습니다!"))));
}

void ASailingSystem::UpgradeMyShipMoveSpeed()
{
	if (Currency < UpgradeCost)
	{
		return;
	}

	UseCurrency(UpgradeCost);
	MyShip->UpgradeMoveSpeed();
}

void ASailingSystem::UpgradeMyShipHandling()
{
	if (Currency < UpgradeCost)
	{
		return;
	}

	UseCurrency(UpgradeCost);
	MyShip->UpgradeHandling();
}

void ASailingSystem::UpgradeMyShipCannonAttack()
{
	if (Currency < UpgradeCost)
	{
		return;
	}

	UseCurrency(UpgradeCost);
	MyShip->UpgradeCannonAttack();
}

float ASailingSystem::GetElapsedTime() const
{
	return ElapsedTime;
}

bool ASailingSystem::IsAllMyCharactersDead() const
{
	bool bIsAllMyCharactersDead = true;

	for (const auto MyCharacter : MyCharacters)
	{
		if (MyCharacter->GetCurrentPlayerState() != UserState::DEAD)
		{
			bIsAllMyCharactersDead = false;
			break;
		}
	}
	
	return bIsAllMyCharactersDead;
}

bool ASailingSystem::IsReachedDestination() const
{
	const FVector DestinationLocation = Destination->GetActorLocation();
	const FVector MyShipLocation = MyShip->GetActorLocation();
	const double Distance = FVector::Dist(DestinationLocation, MyShipLocation);

	return Distance < DistanceToDestination;
}

AMyShip* ASailingSystem::GetMyShip() const
{
	return MyShip;
}

void ASailingSystem::SetMyShip()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyShip::StaticClass(), FoundActors);
	if (FoundActors.Num() > 0)
	{
		MyShip = Cast<AMyShip>(FoundActors[0]);
	}
	InitLocation = MyShip->GetActorLocation();
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
		AEnemyShip* CastedEnemyShip = Cast<AEnemyShip>(FoundEnemyShip);
		CastedEnemyShip->EnemyShipDieDelegate.BindUObject(this, &ASailingSystem::OnEnemyShipDie);
		CastedEnemyShip->SpawnEnemyDelegate.BindUObject(this, &ASailingSystem::OnEnemiesSpawned);
		EnemyShips.Add(CastedEnemyShip);
	}
}

void ASailingSystem::SetDestination()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADestination::StaticClass(), FoundActors);
	if (FoundActors.Num() > 0)
	{
		Destination = Cast<ADestination>(FoundActors[0]);
	}
}

float ASailingSystem::DestinationProgress()
{
	if(Destination && MyShip)
	{
		const FVector DestinationLocation = Destination->GetActorLocation();
		const FVector MyShipLocation = MyShip->GetActorLocation();
		const float TotalDistance = FVector::Dist(InitLocation, DestinationLocation); // 전체 거리
		const float CurrentDistance = FVector::Dist(DestinationLocation, MyShipLocation);

		// 진행도 계산
		Progress = 1.0f - (CurrentDistance / TotalDistance);
		Progress = FMath::Clamp(Progress, 0.0f, 1.0f);
	}
	else
	{
		SetMyShip();
		SetDestination();
	}
	
	return Progress;
}

void ASailingSystem::MulticastRPC_Caution_Implementation(const FText& Text) const
{
	const auto PlayerController = GetWorld()->GetFirstPlayerController();
	const auto InGameHUD = Cast<AMyIngameHUD>(PlayerController->GetHUD());
	InGameHUD->ShowPopupCaution(Text);
}

// nullable
AMyCharacter* ASailingSystem::FindNearestMyCharacter(const AEnemy* Enemy) const
{
	AMyCharacter* NearestMyCharacter = nullptr;

	for (const auto MyCharacter : MyCharacters)
	{
		if (MyCharacter->GetCurrentPlayerState() == UserState::DEAD)
		{
			continue;
		}

		if (NearestMyCharacter == nullptr)
		{
			NearestMyCharacter = MyCharacter;
		}
		else
		{
			if (const auto Distance = FVector::Dist(Enemy->GetActorLocation(), MyCharacter->GetActorLocation());
				Distance < FVector::Dist(Enemy->GetActorLocation(), NearestMyCharacter->GetActorLocation()))
			{
				NearestMyCharacter = MyCharacter;
			}
		}
	}

	return NearestMyCharacter;
}
