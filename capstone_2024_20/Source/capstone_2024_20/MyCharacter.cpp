#include "MyCharacter.h"

#include "CharacterChangerComponent.h"
#include "MyAudioInstance.h"
#include "MyPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "Enemy/Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"


class AStaticMeshActor;
// Sets default values
AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	TextWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
	TextWidget->SetupAttachment(GetMesh());
	TextWidget->SetRelativeLocation(FVector(-60.0f,0.0f,180.0f));
	TextWidget->SetWidgetSpace(EWidgetSpace::Screen);

	NamePlateWidget = CreateDefaultSubobject<UNamePlateWidgetComponent>(TEXT("NICKNAMEWIDGET"));
	NamePlateWidget->SetupAttachment(RootComponent);
	
	CharacterChangerComponent = CreateDefaultSubobject<UCharacterChangerComponent>(TEXT("CharacterChanger"));
	CharacterChangerComponent->SetIsReplicated(true);
	
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/WidgetBlueprints/NewWidgetBlueprint"));
	if(UI_HUD.Succeeded())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("UMG Success"));
		TextWidget->SetWidgetClass(UI_HUD.Class);
		TextWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
		TextWidget->SetVisibility(false);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("UMG Failed"));
	}
	
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

	SetMaxHP(10);
	SetCurrentHP(10);

	
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	check(GEngine != nullptr);

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using FPSCharacter."));

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this,&AMyCharacter::BeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AMyCharacter::EndOverlap);

	FTimerHandle timer;
	GetWorld()->GetTimerManager().SetTimer(timer,this,&AMyCharacter::SetNamePlate, 5.0f, false);


	CharacterChangerComponent->Change(GetGameInstance<UMyAudioInstance>()->GetCharacterType());
}





// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//충돌 처리
void AMyCharacter::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if(CurrentPlayerState != UserState::DRAGGING)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Hit"));
		if(IsLocallyControlled())
			TextWidget->SetVisibility(true);
		bIsOverlap = true;
	
		for (const FString& Tag : ObjectList)
		{
			if (OtherComp->ComponentTags.Contains(Tag))
			{
				CurrentHitObjectName = Tag;
				break;
			}
		}

		CurrentHitObject = Cast<AMyObject>(OtherActor);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, CurrentHitObject->GetName());
	}
		
}

void AMyCharacter::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(CurrentPlayerState != UserState::DRAGGING)
	{
		if(OtherComp->ComponentTags.Contains(TEXT("Object")))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Hit Out"));
			TextWidget->SetVisibility(false);
			bIsOverlap = false;
		}
		CurrentHitObject = Cast<AMyObject>(this);
	}
}


void AMyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyCharacter, bIsSleeping);
}



void AMyCharacter::SetNamePlate()
{
	NamePlateWidget->SetName(GetPlayerState()->GetPlayerName());
}


bool AMyCharacter::GetIsOverLap()
{
	return bIsOverlap;
}

bool AMyCharacter::GetIsSleeping()
{
	return bIsSleeping;
}

void AMyCharacter::SetIsSleeping(bool b)
{
	bIsSleeping = b;

	//SetActorRotation(MyRotation);
}

void AMyCharacter::ServerRPC_SetIsSleeping_Implementation(bool b)
{
	bIsSleeping = b;
}


void AMyCharacter::SetTextWidgetVisible(bool b)
{
	TextWidget->SetVisibility(b);
}

bool AMyCharacter::GetTextWidgetVisible()
{
	return TextWidget->IsVisible();
}


AMyObject* AMyCharacter::GetCurrentHitObject()
{
	return CurrentHitObject;
}


FString AMyCharacter::GetCurrentHitObjectName()
{
	return CurrentHitObjectName;
}

void AMyCharacter::SetCurrentCarryObject(AActor* obj)
{
	CurrentCarryObject = obj;
}

void AMyCharacter::SetPlayerState(UserState NewPlayerState)
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

void AMyCharacter::DropObject(AActor* ship)
{
	CurrentHitObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	CurrentHitObject->AttachToActor(ship, FAttachmentTransformRules::KeepWorldTransform);

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

void AMyCharacter::Attack() const
{
	if (EnemyInAttackRange != nullptr)
	{
		// Todo@autumn - Need to change the damage value
		EnemyInAttackRange->Damage(1);
	}
}



void AMyCharacter::SetEnemyInAttackRange(AEnemy* Enemy)
{
	MulticastRPC_SetEnemyInAttackRange(Enemy);
}

void AMyCharacter::MulticastRPC_SetEnemyInAttackRange_Implementation(AEnemy* Enemy)
{
	EnemyInAttackRange = Enemy;
}










