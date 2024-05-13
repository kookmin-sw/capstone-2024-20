// Fill out your copyright notice in the Description page of Project Settings.


#include "MirrorActor.h"

#include "EngineUtils.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"

AMirrorActor::AMirrorActor()
{
	LongInteractionThreshold = 2.0f;
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(RootComponent);
}

void AMirrorActor::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ACameraActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ACameraActor* CameraActor = *ActorItr;
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
	
	SubCameraActor->SetActorLocation(CameraComponent->GetComponentLocation());
	SubCameraActor->SetActorRotation(CameraComponent->GetComponentRotation());
	GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(SubCameraActor, 2.0f);
}

void AMirrorActor::InteractionEnter()
{
	Super::InteractionEnter();
}

