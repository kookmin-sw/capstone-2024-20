#include "Enemy.h"
#include "EnemyAnimInstance.h"
#include "../MyCharacter.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"

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

	// Todo@autumn - This is a temporary solution, replace it with data.
	SetMaxHP(2);
	SetCurrentHP(2);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemy::Die()
{
	IHP::Die();
	EnemyDieDelegate.Execute(this);
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

void AEnemy::Attack(AMyCharacter* MyCharacter)
{
	ServerRPC_Attack(MyCharacter);
}

void AEnemy::ServerRPC_Attack_Implementation(AMyCharacter* MyCharacter)
{
	MyCharacter->Damage(1); // Todo@autumn - This is a temporary value, replace it with data.
	CurrentAttackCooldown = AttackCooldown;
	MultiCastRPC_Attack(MyCharacter);
}

void AEnemy::MultiCastRPC_Attack_Implementation(AMyCharacter* MyCharacter)
{
	UEnemyAnimInstance* AnimInstance = Cast<UEnemyAnimInstance>(SkeletalMesh->GetAnimInstance());
	AnimInstance->bIsAttacking = true;
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

bool AEnemy::CanAttack() const
{
	return CurrentAttackCooldown <= 0;
}
