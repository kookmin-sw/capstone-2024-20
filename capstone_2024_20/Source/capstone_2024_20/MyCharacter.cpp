#include "MyCharacter.h"
#include "CharacterChangerComponent.h"
#include "MyAudioInstance.h"
#include "MyIngameHUD.h"
#include "MyPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "Enemy/Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Pirate/PirateAnimInstance.h"
#include "WidgetBlueprint/PopupInteraction.h"

class AStaticMeshActor;

AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	TextWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
	TextWidget->SetupAttachment(RootComponent);
	TextWidget->SetWidgetSpace(EWidgetSpace::Screen);

	NamePlateWidget = CreateDefaultSubobject<UNamePlateWidgetComponent>(TEXT("NICKNAMEWIDGET"));
	NamePlateWidget->SetupAttachment(RootComponent);
	
	CharacterChangerComponent = CreateDefaultSubobject<UCharacterChangerComponent>(TEXT("CharacterChanger"));
	CharacterChangerComponent->SetIsReplicated(true);
	
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/WidgetBlueprints/NewWidgetBlueprint"));
	TextWidget->SetWidgetClass(UI_HUD.Class);
	TextWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	TextWidget->SetVisibility(false);
	
	RootComponent = GetCapsuleComponent();

	GetMesh()->AttachToComponent(GetCapsuleComponent(),FAttachmentTransformRules::KeepRelativeTransform);

	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);
	GetCapsuleComponent()->SetCapsuleRadius(20.0f);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f,0.0f,-88.0f), FRotator(0.0f, 0.0f, 0.0f));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 1000.f, 0.f);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	MyInGameHUD = Cast<AMyIngameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	PopupInteraction = Cast<UPopupInteraction>(TextWidget->GetUserWidgetObject());

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this,&AMyCharacter::BeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AMyCharacter::EndOverlap);

	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer,this,&AMyCharacter::SetNamePlate, 5.0f, false);
	
	CharacterChangerComponent->Change(GetGameInstance<UMyAudioInstance>()->GetCharacterType());

	PirateAnimInstance = Cast<UPirateAnimInstance>(GetMesh()->GetAnimInstance());
	PirateAnimInstance->OnPirateGiveDamageDelegate.BindUObject(this, &AMyCharacter::GiveDamage);
	PirateAnimInstance->OnPirateAttackEndDelegate.BindUObject(this, &AMyCharacter::AttackEnd);

	SetMaxHP(10);
	SetCurrentHP(10);
}

// ReSharper disable once CppParameterMayBeConst
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ! 캐릭터에 Attach한 UI가 Movement에 따라 움직이는 문제가 있어, Tick에서 위치를 업데이트
	TextWidget->SetWorldLocation(GetActorLocation() + FVector(0.0f, 0.0f, -200.0f));
}

void AMyCharacter::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(CurrentPlayerState != UserState::DRAGGING)
	{
		bIsOverlap = true;
	
		for (const FString& Tag : ObjectList)
		{
			if (OtherComp->ComponentTags.Contains(Tag))
			{
				CurrentHitObjectName = Tag;
				break;
			}
		}

		if(IsLocallyControlled())
		{
			TextWidget->SetVisibility(true);
			const FString PopupText = InteractionTextMap.FindRef(CurrentHitObjectName);
			PopupInteraction->SetInteractionText(PopupText);
		}

		CurrentHitObject = Cast<AMyObject>(OtherActor);
	}
}

void AMyCharacter::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(CurrentPlayerState != UserState::DRAGGING)
	{
		if(OtherComp->ComponentTags.Contains(TEXT("Object")))
		{
			TextWidget->SetVisibility(false);
			bIsOverlap = false;
		}
		CurrentHitObject = Cast<AMyObject>(this);
	}
}

// ! 사이트 이펙트를 막기 위함
// ReSharper disable once CppMemberFunctionMayBeConst
void AMyCharacter::GiveDamage()
{
	if (!HasAuthority())
	{
		return;
	}
	
	TArray<AActor*> FoundEnemies;
	AEnemy* EnemyToGiveDamage = nullptr; // ! nullable
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), FoundEnemies);
	float MinDistance = TNumericLimits<float>::Max();

	for (AActor* Enemy : FoundEnemies)
	{
		AEnemy* CastedEnemy = Cast<AEnemy>(Enemy);
		const auto Distance = FVector::Dist(GetActorLocation(), CastedEnemy->GetActorLocation());
		const auto Direction = CastedEnemy->GetActorLocation() - GetActorLocation();

		if (Distance > AttackRange)
		{
			continue;
		}

		if (Distance > MinDistance)
		{
			continue;
		}

		// check if the enemy is in front of the character
		if (const float DotProduct = FVector::DotProduct(Direction.GetSafeNormal(), GetActorForwardVector()); DotProduct < 0.5f)
		{
			continue;
		}

		MinDistance = Distance;
		EnemyToGiveDamage = CastedEnemy;
	}
	
	if (EnemyToGiveDamage != nullptr)
	{
		EnemyToGiveDamage->Damage(1);
	}
}

int32 AMyCharacter::GetMaxHP() const
{
	return MaxHP;
}

int32 AMyCharacter::GetCurrentHP() const
{
	return CurrentHP;
}

void AMyCharacter::SetMaxHP(const int32 NewMaxHP)
{
	if (NewMaxHP < 0)
	{
		MaxHP = 0;
		return;
	}
	
	MaxHP = NewMaxHP;
}

void AMyCharacter::SetCurrentHP(const int32 NewCurrentHP)
{
	if (NewCurrentHP < 0)
	{
		CurrentHP = 0;
		return;
	}
	
	CurrentHP = NewCurrentHP;
}

void AMyCharacter::Heal(const int32 HealAmount)
{
	CurrentHP = FMath::Clamp(CurrentHP + HealAmount, 0, MaxHP);
}

void AMyCharacter::Damage(const int32 DamageAmount)
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
}

void AMyCharacter::Die()
{
	SetPlayerState(UserState::DEAD);
	CurrentReviveCooldown = ReviveCooldown;
	Multicast_Die();
}

void AMyCharacter::Multicast_Die_Implementation() const
{
	if (IsLocallyControlled())
	{
		MyInGameHUD->SetPopupDeadVisibility(true);
	}
}

void AMyCharacter::Revive()
{
	CurrentHP = MaxHP;
	SetPlayerState(UserState::NONE);
	Multicast_Revive();
}

void AMyCharacter::Multicast_Revive_Implementation() const
{
	if (IsLocallyControlled())
	{
		MyInGameHUD->SetPopupDeadVisibility(false);
	}
}

// ReSharper disable once CppParameterMayBeConst
void AMyCharacter::ReduceReviveCooldown(float DeltaTime)
{
	if (CurrentReviveCooldown <= 0.0f)
	{
		return;
	}
	
	CurrentReviveCooldown -= DeltaTime;
	Multicast_ReduceReviveCooldown();
}

void AMyCharacter::Multicast_ReduceReviveCooldown_Implementation()
{
	if (!IsLocallyControlled())
	{
		return;
	}
	
	MyInGameHUD->SetPopupDeadTextByReviveCooldown(CurrentReviveCooldown);
}

bool AMyCharacter::CanRevive() const
{
	return CurrentReviveCooldown <= 0.0f;
}

// Blueprint에서 실행 흐름을 제어하기 위함
// ReSharper disable once CppUE4BlueprintCallableFunctionMayBeConst
bool AMyCharacter::IsAttacking()
{
	return bIsAttacking;
}

// ReSharper disable once CppParameterMayBeConst
void AMyCharacter::ReduceAttackCooldown(float DeltaTime)
{
	if (CurrentAttackCooldown > 0.0f)
	{
		CurrentAttackCooldown -= DeltaTime;
	}
}

bool AMyCharacter::CanAttack() const
{
	return CurrentAttackCooldown <= 0.0f;
}

void AMyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyCharacter, bIsSleeping);
	DOREPLIFETIME(AMyCharacter, CurrentAttackCooldown);
	DOREPLIFETIME(AMyCharacter, MaxHP);
	DOREPLIFETIME(AMyCharacter, CurrentHP);
	DOREPLIFETIME(AMyCharacter, CurrentPlayerState);
	DOREPLIFETIME(AMyCharacter, CurrentReviveCooldown);
	DOREPLIFETIME(AMyCharacter, bIsAttacking);
}

void AMyCharacter::SetNamePlate() const
{
	ServerRPC_SetNamePlate();
}

void AMyCharacter::ServerRPC_SetNamePlate_Implementation() const
{
	for(auto It = GetWorld()->GetPlayerControllerIterator();It;++It)
	{
		if((*It)->GetCharacter() == nullptr)
			return;
		
		AMyCharacter* MyCharacter = Cast<AMyCharacter>((*It)->GetCharacter());
		if(MyCharacter)
		{
			MyCharacter->ClientRPC_SetNamePlate();
		}
	}
}

void AMyCharacter::ClientRPC_SetNamePlate_Implementation()
{
	NamePlateWidget->SetName(GetGameInstance<UMyAudioInstance>()->PlayerName);
}

bool AMyCharacter::GetIsOverLap() const
{
	return bIsOverlap;
}

bool AMyCharacter::GetIsSleeping()
{
	return bIsSleeping;
}

void AMyCharacter::SetIsSleeping(const bool b)
{
	bIsSleeping = b;
}

void AMyCharacter::ServerRPC_SetIsSleeping_Implementation(const bool b)
{
	bIsSleeping = b;
}

void AMyCharacter::SetTextWidgetVisible(const bool b) const
{
	TextWidget->SetVisibility(b);
}

bool AMyCharacter::GetTextWidgetVisible() const
{
	return TextWidget->IsVisible();
}

AMyObject* AMyCharacter::GetCurrentHitObject() const
{
	return CurrentHitObject;
}

FString AMyCharacter::GetCurrentHitObjectName()
{
	return CurrentHitObjectName;
}

void AMyCharacter::SetCurrentCarryObject(AActor* OBJ)
{
	CurrentCarryObject = OBJ;
}

UserState AMyCharacter::GetCurrentPlayerState() const
{
	return CurrentPlayerState;
}

void AMyCharacter::SetPlayerState(const UserState NewPlayerState)
{
	CurrentPlayerState = NewPlayerState;
}

UserState AMyCharacter::GetUserStateNone()
{
	return UserState::NONE;
}

UserState AMyCharacter::GetUserStateCarrying()
{
	return UserState::CARRYING;
}

UserState AMyCharacter::GetUserStateDragging()
{
	return UserState::DRAGGING;
}

UserState AMyCharacter::GetUserStateSleeping()
{
	return UserState::SLEEPING;
}

void AMyCharacter::DestroyCannonBall()
{
	if(CurrentCarryObject)
	{
		CurrentCarryObject->Destroy();
		CurrentCarryObject = nullptr;
	}	
}

void AMyCharacter::DragObject()
{
	CurrentHitObject->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	const FRotator RelativeRotation = FRotator(0.0f, -90.0f, 0.0f); // -90도 회전 (캐릭터 메시도 -90도 회전시켜서 사용중)
	CurrentHitObject->SetActorRelativeRotation(RelativeRotation);

	if (CurrentHitObject)
	{
		TArray<UPrimitiveComponent*> Components;
		CurrentHitObject->GetComponents<UPrimitiveComponent>(Components);

		for (UPrimitiveComponent* Component : Components)
		{
			if (!Component->GetName().Equals(TEXT("Box")))
			{
				Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		}
		CurrentHitObject->SetIsDragging(true);
	}
}

void AMyCharacter::DropObject(AActor* Ship) const
{
	CurrentHitObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	CurrentHitObject->AttachToActor(Ship, FAttachmentTransformRules::KeepWorldTransform);

	if (CurrentHitObject)
	{
		TArray<UPrimitiveComponent*> Components;
		CurrentHitObject->GetComponents<UPrimitiveComponent>(Components);

		for (UPrimitiveComponent* Component : Components)
		{
			Component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
		CurrentHitObject->SetIsDragging(false);
	}
}

void AMyCharacter::Attack()
{
	ServerRPC_Attack();
}

void AMyCharacter::AttackEnd()
{
	if (!HasAuthority())
	{
		return;
	}
	
	bIsAttacking = false;
}

void AMyCharacter::ServerRPC_Attack_Implementation()
{
	CurrentAttackCooldown = AttackCooldown;
	bIsAttacking = true;
}

float AMyCharacter::GetHPPercent() const
{
	return static_cast<float>(GetCurrentHP()) / static_cast<float>(GetMaxHP());
}
