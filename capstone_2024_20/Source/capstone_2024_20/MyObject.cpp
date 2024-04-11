// Fill out your copyright notice in the Description page of Project Settings.


#include "MyObject.h"

#include "Net/UnrealNetwork.h"

// Sets default values
AMyObject::AMyObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

// Called when the game starts or when spawned
void AMyObject::BeginPlay()
{
	Super::BeginPlay();

	TargetRotation = GetActorRotation();
	
}

// Called every frame
void AMyObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(IsDragging)
	{
		SetActorRotation(TargetRotation);
	}
	else
	{
		SetActorRelativeRotation(TargetRotation);
	}

}

void AMyObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyObject, IsDragging);
	DOREPLIFETIME(AMyObject, TargetRotation);
}



bool AMyObject::GetIsDragging()
{
	return IsDragging;
}

void AMyObject::SetIsDragging(bool b)
{
	IsDragging = b;
}

void AMyObject::MulticastRPC_TurnOffCollision_Implementation()
{
	TArray<UPrimitiveComponent*> Components;
    GetComponents<UPrimitiveComponent>(Components);

    for (UPrimitiveComponent* Component : Components)
    {
    	if (!Component->GetName().Equals(TEXT("Box")))
    	{
    		Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    	}
    }
}


void AMyObject::MulticastRPC_TurnOnCollision_Implementation()
{
	TArray<UPrimitiveComponent*> Components;
    GetComponents<UPrimitiveComponent>(Components);

    for (UPrimitiveComponent* Component : Components)
    {
    	Component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    }
}


