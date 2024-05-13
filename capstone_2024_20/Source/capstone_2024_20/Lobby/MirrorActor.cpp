// Fill out your copyright notice in the Description page of Project Settings.


#include "MirrorActor.h"

#include "CharacterChangerWidgetComponent.h"
#include "EngineUtils.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "capstone_2024_20/MyAudioInstance.h"

AMirrorActor::AMirrorActor()
{
	LongInteractionThreshold = 2.0f;
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(RootComponent);

	CharacterChangerWidgetComponent = CreateDefaultSubobject<UCharacterChangerWidgetComponent>(
		TEXT("CharacterChangerWidgetComponent"));
	CharacterChangerWidgetComponent->SetupAttachment(RootComponent);
}

void AMirrorActor::BeginPlay()
{
	Super::BeginPlay();

	CharacterChangerWidgetComponent->GetWidget()->SetVisibility(ESlateVisibility::Hidden);

	for (TActorIterator<ACameraActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ACameraActor* CameraActor = *ActorItr;
		GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Emerald,
		                                 CameraActor->GetName());
		if (CameraActor && CameraActor->GetName() == "SubCameraActor")
		{
			SubCameraActor = CameraActor;
			break;
		}
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
	GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(SubCameraActor, 0.3f);
}

void AMirrorActor::InteractionEnter()
{
	Super::InteractionEnter();
}

void AMirrorActor::SetCharacterType(int32 CharacterType)
{
	int32 CharacterTypeClamp = FMath::Clamp(CharacterType, 0, 3);
	const ECharacterType NewEnumValue = static_cast<ECharacterType>(CharacterTypeClamp);

	UMyAudioInstance* MyAudioInstance = GetGameInstance<UMyAudioInstance>();
	MyAudioInstance->SetCharacterType(NewEnumValue);
}

void AMirrorActor::Next()
{
	UMyAudioInstance* MyAudioInstance = GetGameInstance<UMyAudioInstance>();

	int32 EnumAsInt = MyAudioInstance->GetCharacterType();
	SetCharacterType(EnumAsInt + 1);
}

void AMirrorActor::Prev()
{
	UMyAudioInstance* MyAudioInstance = GetGameInstance<UMyAudioInstance>();

	int32 EnumAsInt = MyAudioInstance->GetCharacterType();
	SetCharacterType(EnumAsInt - 1);
}

void AMirrorActor::Exit()
{
}
