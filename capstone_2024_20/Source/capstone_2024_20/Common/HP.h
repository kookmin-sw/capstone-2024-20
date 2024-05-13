#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HP.generated.h"

UINTERFACE()
class UHP : public UInterface
{
	GENERATED_BODY()
};

class CAPSTONE_2024_20_API IHP
{
	GENERATED_BODY()

public:
	int32 MaxHP = 0;
	int32 CurrentHP = 0;

	int32 GetMaxHP() const;
	int32 GetCurrentHP() const;

	void SetMaxHP(const int32 NewMaxHP);
	void SetCurrentHP(const int32 NewCurrentHP);

	void Heal(const int32 HealAmount);
	void Damage(const int32 DamageAmount);

	virtual void Die();
	virtual void Revive();
};
