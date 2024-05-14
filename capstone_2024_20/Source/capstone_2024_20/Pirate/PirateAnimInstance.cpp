#include "PirateAnimInstance.h"

#include "Net/UnrealNetwork.h"

void UPirateAnimInstance::OnAttackEnd()
{
	bIsAttacking = false;
}

void UPirateAnimInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPirateAnimInstance, bIsAttacking);
}
