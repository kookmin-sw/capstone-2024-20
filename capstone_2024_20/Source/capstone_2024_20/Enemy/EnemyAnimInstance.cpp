#include "EnemyAnimInstance.h"
#include "Net/UnrealNetwork.h"

void UEnemyAnimInstance::OnAttackEnd()
{
	bIsAttacking = false;
}

// ! 사이드 이펙트를 막기 위함
// ReSharper disable once CppUE4BlueprintCallableFunctionMayBeConst
void UEnemyAnimInstance::OnGiveDamage()
{
	OnGiveDamageDelegate.Execute();
}

void UEnemyAnimInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEnemyAnimInstance, bIsAttacking);
}
