#include "EnemyAnimInstance.h"
#include "Net/UnrealNetwork.h"

void UEnemyAnimInstance::OnAttackEnd()
{
	bIsAttacking = false;
}

void UEnemyAnimInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEnemyAnimInstance, bIsAttacking);
}
