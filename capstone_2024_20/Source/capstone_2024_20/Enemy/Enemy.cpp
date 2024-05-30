#include "Enemy.h"
#include "EnemyAnimInstance.h"
#include "../MyCharacter.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "capstone_2024_20/WidgetBlueprint/PopupEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AEnemy::AEnemy(): SkeletalMesh(nullptr)
{
	// TODO@autumn - This is a temporary mesh, replace it with the actual mesh from data
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetSkeletalMesh(LoadObject<USkeletalMesh>(nullptr, TEXT("/Script/Engine.SkeletalMesh'/Game/GameObjects/Enemy/Base.Base'")));
	RootComponent = SkeletalMesh;

	// 적이 해적을 향해 이동 시 물리적 충돌을 발생시키지 않도록 하기 위함
	SkeletalMesh->SetSimulatePhysics(false);
	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SkeletalMesh->SetCollisionProfileName(TEXT("NoCollision"));

	NavInvoker = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavInvoker"));

	SkeletalMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	SkeletalMesh->SetAnimInstanceClass(LoadObject<UClass>(nullptr, TEXT("/Script/Engine.AnimBlueprint'/Game/GameObjects/Enemy/ABP_Enemy.ABP_Enemy_C'")));

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/WidgetBlueprints/Enemy/BP_PopupEnemy.BP_PopupEnemy_C'"));
	PopupEnemyWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	PopupEnemyWidgetComponent->SetWidgetClass(WidgetClass.Class);
	PopupEnemyWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	PopupEnemyWidgetComponent->SetupAttachment(RootComponent);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	SetMaxHP(2);
	SetCurrentHP(2);

	PopupEnemyWidget = Cast<UPopupEnemy>(PopupEnemyWidgetComponent->GetUserWidgetObject());
	PopupEnemyWidget->SetHPProgressBarPercent(1);
	
	AnimInstance = Cast<UEnemyAnimInstance>(SkeletalMesh->GetAnimInstance());
	AnimInstance->OnGiveDamageDelegate.BindUObject(this, &AEnemy::GiveDamage);
}

void AEnemy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEnemy, MaxHP);
	DOREPLIFETIME(AEnemy, CurrentHP);
}

int32 AEnemy::GetMaxHP() const
{
	return MaxHP;
}

int32 AEnemy::GetCurrentHP() const
{
	return CurrentHP;
}

void AEnemy::SetMaxHP(const int32 NewMaxHP)
{
	if (NewMaxHP < 0)
	{
		MaxHP = 0;
		return;
	}
	
	MaxHP = NewMaxHP;
}

void AEnemy::SetCurrentHP(const int32 NewCurrentHP)
{
	if (NewCurrentHP < 0)
	{
		CurrentHP = 0;
		return;
	}
	
	CurrentHP = NewCurrentHP;
}

void AEnemy::Heal(const int32 HealAmount)
{
	CurrentHP = FMath::Clamp(CurrentHP + HealAmount, 0, MaxHP);
}

void AEnemy::Damage(const int32 DamageAmount)
{
	if (CurrentHP == 0)
	{
		return;
	}
	
	CurrentHP = FMath::Clamp(CurrentHP - DamageAmount, 0, MaxHP);

	if (CurrentHP == 0)
	{
		Die();
	}

	MulticastRPC_Damage();
}

void AEnemy::Die()
{
	EnemyDieDelegate.Execute(this);
}

void AEnemy::MulticastRPC_Damage_Implementation()
{
	PopupEnemyWidget->SetHPProgressBarPercent(static_cast<float>(CurrentHP) / MaxHP);
}

void AEnemy::MoveToMyCharacter(const AMyCharacter* MyCharacter)
{
	const auto MyCharacterLocation = MyCharacter->GetActorLocation();
	if (const auto Direction = MyCharacterLocation - GetActorLocation(); Direction.Size() < DistanceToMyCharacter) // Todo@autumn - This is a temporary solution, replace it with data.
	{
		return;
	}

	const UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	const auto Path = NavSys->FindPathToLocationSynchronously(GetWorld(), GetActorLocation(), MyCharacterLocation);
	if (!Path->IsValid () || Path->PathPoints.Num() < 2)
	{
		return;
	}

	const FVector NextPoint = Path->PathPoints[1];
	const FVector DirectionToNextPoint = NextPoint - GetActorLocation();
	const FVector NewLocation = GetActorLocation() + DirectionToNextPoint.GetSafeNormal() * MoveSpeed * GetWorld()->GetDeltaSeconds();

	SetActorLocation(NewLocation);
	SetActorRotation(DirectionToNextPoint.Rotation());
}

void AEnemy::Attack()
{
	ServerRPC_Attack();
}

void AEnemy::ServerRPC_Attack_Implementation()
{
	CurrentAttackCooldown = AttackCooldown;
	MultiCastRPC_Attack();
}

void AEnemy::MultiCastRPC_Attack_Implementation()
{
	AnimInstance->bIsAttacking = true;
}

// ! 바인딩하기 위함
// ReSharper disable once CppMemberFunctionMayBeConst
void AEnemy::GiveDamage()
{
	TArray<AActor*> MyCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyCharacter::StaticClass(), MyCharacters);
	
	AMyCharacter* ClosestMyCharacter = nullptr; // ! nullable
	float ClosestDistance = TNumericLimits<float>::Max();
	for (const auto MyCharacter : MyCharacters)
	{
		const auto CastedMyCharacter = Cast<AMyCharacter>(MyCharacter);
		const float Distance = FVector::Dist(CastedMyCharacter->GetActorLocation(), GetActorLocation());
		const FVector Direction = CastedMyCharacter->GetActorLocation() - GetActorLocation();
		
		if (Distance > ClosestDistance)
		{
			continue;
		}
		
		if (Direction.Size() > DistanceToMyCharacter)
		{
			continue;
		}

		// check if the character is in front of the enemy
		if (const float DotProduct = FVector::DotProduct(Direction.GetSafeNormal(), GetActorForwardVector()); DotProduct < 0.5f)
		{
			continue;
		}

		ClosestDistance = Distance;
		ClosestMyCharacter = CastedMyCharacter;
	}

	if (ClosestMyCharacter != nullptr)
	{
		ClosestMyCharacter->Damage(1);
	}
}

void AEnemy::ReduceCurrentAttackCooldown(float DeltaTime)
{
	if (CurrentAttackCooldown > 0)
	{
		CurrentAttackCooldown -= DeltaTime;
	}
}

float AEnemy::GetDistanceToMyCharacter() const
{
	return DistanceToMyCharacter;
}

bool AEnemy::CanMove() const
{
	return !AnimInstance->bIsAttacking;
}

bool AEnemy::CanAttack() const
{
	return CurrentAttackCooldown <= 0;
}
