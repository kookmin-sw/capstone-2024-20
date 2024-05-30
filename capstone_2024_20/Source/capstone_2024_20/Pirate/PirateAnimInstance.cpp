#include "PirateAnimInstance.h"

void UPirateAnimInstance::OnAttackEnd() const
{
	OnPirateAttackEndDelegate.Execute();
}

// ! 사이드 이펙트를 막기 위함
// ReSharper disable once CppUE4BlueprintCallableFunctionMayBeConst
void UPirateAnimInstance::OnGiveDamage()
{
	OnPirateGiveDamageDelegate.Execute();
}
