#include "HP.h"

int32 IHP::GetMaxHP() const
{
	return MaxHP;
}

int32 IHP::GetCurrentHP() const
{
	return CurrentHP;
}

void IHP::SetMaxHP(const int32 NewMaxHP)
{
	if (NewMaxHP < 0)
	{
		MaxHP = 0;
		return;
	}
	
	MaxHP = NewMaxHP;
}

void IHP::SetCurrentHP(const int32 NewCurrentHP)
{
	if (NewCurrentHP < 0)
	{
		CurrentHP = 0;
		return;
	}
	
	CurrentHP = NewCurrentHP;
}

void IHP::Heal(const int32 HealAmount)
{
	CurrentHP = FMath::Clamp(CurrentHP + HealAmount, 0, MaxHP);
	GEngine->AddOnScreenDebugMessage(
	-1, 2.0f, FColor::Red, TEXT("회복됨"));
}

void IHP::Damage(const int32 DamageAmount)
{
	CurrentHP = FMath::Clamp(CurrentHP - DamageAmount, 0, MaxHP);

	if (CurrentHP == 0)
	{
		Die();
	}
}

void IHP::Die()
{
	// do nothing, override if needed
}
