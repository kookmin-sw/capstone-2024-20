// Fill out your copyright notice in the Description page of Project Settings.


#include "MirrorActor.h"

#include "CapCharacter.h"
#include "CharacterChangerWidgetComponent.h"
#include "CharacterChangeWidget.h"
#include "EngineUtils.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "capstone_2024_20/MyAudioInstance.h"
#include "Components/ArrowComponent.h"
#include "Components/Button.h"
#include "GameFramework/CharacterMovementComponent.h"

AMirrorActor::AMirrorActor()
{
	LongInteractionThreshold = 1.0f;
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(RootComponent);

	CharacterPositionArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("CharacterPositionArrow"));
	CharacterPositionArrow->SetupAttachment(RootComponent);

	CharacterChangerWidgetComponent = CreateDefaultSubobject<UCharacterChangerWidgetComponent>(
		TEXT("CharacterChangerWidgetComponent"));
	CharacterChangerWidgetComponent->SetupAttachment(RootComponent);

	ExplainText = TEXT("캐릭터 변경");
}

void AMirrorActor::BeginPlay()
{
	Super::BeginPlay();

	CharacterChangerWidgetComponent->GetWidget()->SetVisibility(ESlateVisibility::Hidden);

	for (TActorIterator<ACameraActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ACameraActor* CameraActor = *ActorItr;
		if (CameraActor && CameraActor->GetName() == "SubCameraActor")
		{
			SubCameraActor = CameraActor;
			break;
		}
	}

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (PlayerController)
	{
		CapCharacter = Cast<ACapCharacter>(PlayerController->GetCharacter());
		PlayerController->InputComponent->BindKey(EKeys::A, IE_Pressed,
												  this, &ThisClass::Prev);
		PlayerController->InputComponent->BindKey(EKeys::D, IE_Pressed,
												  this, &ThisClass::Next);
		PlayerController->InputComponent->BindKey(EKeys::R, IE_Pressed,
												  this, &ThisClass::Exit);
	}
}

void AMirrorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMirrorActor::InteractionLongEnter()
{
	Super::InteractionLongEnter();
	CharacterChangerWidgetComponent->GetWidget()->SetVisibility(ESlateVisibility::Visible);
	SubCameraActor->SetActorLocation(CameraComponent->GetComponentLocation());
	SubCameraActor->SetActorRotation(CameraComponent->GetComponentRotation());

	OriginalTarget = GetWorld()->GetFirstPlayerController()->GetViewTarget();
	GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(SubCameraActor, 0.3f);

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (PlayerController)
	{
		CapCharacter = Cast<ACapCharacter>(PlayerController->GetCharacter());
		CapCharacter->SetIsMovement(false);
		ChangeCharacterLocationAndRotation();
	}
}

void AMirrorActor::InteractionEnter()
{
	Super::InteractionEnter();
}

void AMirrorActor::SetCharacterType(int32 CharacterType)
{
	int32 CharacterTypeClamp = FMath::Clamp(CharacterType, 0, 2);
	const ECharacterType NewEnumValue = static_cast<ECharacterType>(CharacterTypeClamp);

	UMyAudioInstance* MyAudioInstance = GetGameInstance<UMyAudioInstance>();
	MyAudioInstance->SetCharacterType(NewEnumValue);

	if (CharacterTypeClamp <= 0)
	{
		CharacterChangerWidgetComponent->CharacterChangeWidget->PrevButton->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		CharacterChangerWidgetComponent->CharacterChangeWidget->PrevButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (CharacterTypeClamp >= 2)
	{
		CharacterChangerWidgetComponent->CharacterChangeWidget->NextButton->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		CharacterChangerWidgetComponent->CharacterChangeWidget->NextButton->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMirrorActor::Next()
{
	if (IsValid(CapCharacter) == false)
		return;
	
	if (CapCharacter->GetIsMovement() == true)
		return;

	UMyAudioInstance* MyAudioInstance = GetGameInstance<UMyAudioInstance>();

	int32 EnumAsInt = MyAudioInstance->GetCharacterType();
	SetCharacterType(EnumAsInt + 1);
}

void AMirrorActor::Prev()
{
	if (IsValid(CapCharacter) == false)
		return;
	
	if (CapCharacter->GetIsMovement() == true)
		return;

	UMyAudioInstance* MyAudioInstance = GetGameInstance<UMyAudioInstance>();

	int32 EnumAsInt = MyAudioInstance->GetCharacterType();
	SetCharacterType(EnumAsInt - 1);
}

void AMirrorActor::Exit()
{
	if (IsValid(CapCharacter) == false)
		return;
	
	if (CapCharacter->GetIsMovement() == true)
		return;
	
	CharacterChangerWidgetComponent->GetWidget()->SetVisibility(ESlateVisibility::Hidden);
	CapCharacter->SetIsMovement(true);

	GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(OriginalTarget, 0.3f);

	CapCharacter->GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
}

void AMirrorActor::ChangeCharacterLocationAndRotation()
{
	FVector Location = CapCharacter->GetActorLocation();
	Location.X = CharacterPositionArrow->GetComponentLocation().X;
	Location.Y = CharacterPositionArrow->GetComponentLocation().Y;
	CapCharacter->ServerRPC_SetLocationAndRotation(Location, CharacterPositionArrow->GetComponentRotation());
}
