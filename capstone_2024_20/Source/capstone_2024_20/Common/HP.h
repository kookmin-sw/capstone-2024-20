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
	virtual int32 GetMaxHP() const = 0;
	virtual int32 GetCurrentHP() const = 0;

	virtual void SetMaxHP(const int32 NewMaxHP) = 0;
	virtual void SetCurrentHP(const int32 NewCurrentHP) = 0;

	virtual void Heal(const int32 HealAmount) = 0;
	virtual void Damage(const int32 DamageAmount) = 0;

	virtual void Die() = 0;
};
