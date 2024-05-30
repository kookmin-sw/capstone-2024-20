#include "PirateAnimInstance.h"
#include "Net/UnrealNetwork.h"

void UPirateAnimInstance::OnAttackEnd()
{
	bIsAttacking = false;
}

// ! 사이드 이펙트를 막기 위함
// ReSharper disable once CppUE4BlueprintCallableFunctionMayBeConst
void UPirateAnimInstance::OnGiveDamage()
{
	OnPirateGiveDamageDelegate.Execute();
}

void UPirateAnimInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPirateAnimInstance, bIsAttacking);
}
