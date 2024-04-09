#include "MyCharacter.h"

#include "MyPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"


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
	
	M_SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	M_CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	M_MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = GetCapsuleComponent();

	GetMesh()->AttachToComponent(GetCapsuleComponent(),FAttachmentTransformRules::KeepRelativeTransform);
	M_SpringArmComponent->SetupAttachment(GetCapsuleComponent());
	M_CameraComponent->SetupAttachment(M_SpringArmComponent);
	M_MeshComponent->SetupAttachment(GetCapsuleComponent());

	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);
	GetCapsuleComponent()->SetCapsuleRadius(20.0f);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f,0.0f,-88.0f), FRotator(0.0f, -90.0f, 0.0f));

	M_SpringArmComponent->TargetArmLength = 1000.0f;
	M_SpringArmComponent->SetRelativeLocationAndRotation(FVector(0.0f,0.0f,40.0f), FRotator(-25.0f,0.0f,0.0f));
	//M_SpringArmComponent->bDoCollisionTest = false;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder'"));
	if(StaticMesh.Object)
	{
		M_MeshComponent->SetStaticMesh(StaticMesh.Object);
		M_MeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f));
	}

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
	GetWorld()->GetTimerManager().SetTimer(timer,this,&AMyCharacter::SetNamePlate, 2.0f, false);
}





// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// debug
	if(bIsChanging)
	{
		M_SpringArmComponent->TargetArmLength = FMath::FInterpTo(M_SpringArmComponent->TargetArmLength, TargetArmLength, DeltaTime, ChangeSpeed);

		FRotator CurrentRotation = M_SpringArmComponent->GetRelativeRotation();
		FRotator newRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, ChangeSpeed);
		M_SpringArmComponent->SetRelativeRotation(newRotation);
		

		if(FMath::IsNearlyEqual(M_SpringArmComponent->TargetArmLength, TargetArmLength, 0.01f)
			&& M_SpringArmComponent->GetComponentRotation().Equals(TargetRotation, 0.01f))
			bIsChanging = false;
	}

	GetMesh()->SetWorldRotation(MeshRotation);
	
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

	DOREPLIFETIME(AMyCharacter, MeshRotation);
}


void AMyCharacter::ServerRPC_MeshRotation_Implementation(FRotator NewRotation)
{
	MeshRotation = NewRotation;
}

void AMyCharacter::SetNamePlate()
{
	NamePlateWidget->SetName(GetPlayerState()->GetPlayerName());
}


bool AMyCharacter::GetIsOverLap()
{
	return bIsOverlap;
}

void AMyCharacter::SetTextWidgetVisible(bool b)
{
	TextWidget->SetVisibility(b);
}

bool AMyCharacter::GetTextWidgetVisible()
{
	return TextWidget->IsVisible();
}

void AMyCharacter::SetIsChanging(float length, FRotator rot, bool b)
{
	TargetArmLength = length;
	TargetRotation = rot;
	bIsChanging = b;
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

unsigned int AMyCharacter::GetPlayerHP()
{
	return PlayerHP;
}

void AMyCharacter::SetPlayerHP(unsigned int hp)
{
	PlayerHP = hp;
}


void AMyCharacter::IncreaseHP(int plusHP)
{
	if(PlayerHP + plusHP > PlayerMaxHP)
		PlayerHP = PlayerMaxHP;
	else
		PlayerHP += plusHP;
}

void AMyCharacter::DecreaseHP(unsigned int minusHP)
{
	if(PlayerHP <= minusHP)
	{
		PlayerHP = 0;
		PlayerDead();
	}
	else
	{
		PlayerHP -= minusHP;
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Player HP: %u"), PlayerHP));
	
}

unsigned int AMyCharacter::GetPlayerMaxHP()
{
	return PlayerMaxHP;
}

void AMyCharacter::SetPlayerMaxHP(unsigned int hp)
{
	PlayerMaxHP = hp;
}


void AMyCharacter::IncreaseMaxHP(int plusHP)
{
	PlayerMaxHP += plusHP;
}

void AMyCharacter::DecreaseMaxHP(int minusHP)
{
	PlayerMaxHP -= minusHP;
	if(PlayerHP > PlayerMaxHP)
		PlayerHP = PlayerMaxHP;
}

void AMyCharacter::PlayerDead()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Player Dead"));
}










