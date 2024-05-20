#include "MyObject.h"
#include "Net/UnrealNetwork.h"

AMyObject::AMyObject() : CanBeOperated(false)
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void AMyObject::BeginPlay()
{
	Super::BeginPlay();

	TargetRotation = GetActorRotation();
}

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

void AMyObject::Interact()
{
	if (CanBeOperated)
	{
		Operate();
	}
}

void AMyObject::InteractOnServer()
{
	if (CanBeOperatedOnServer)
	{
		OperateOnServer();
	}	
}

void AMyObject::Operate()
{
	// do nothing, override if needed
}

void AMyObject::OperateOnServer()
{
	// do nothing, override if needed
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
