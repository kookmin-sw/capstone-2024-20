#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PirateAnimInstance.generated.h"

DECLARE_DELEGATE(FOnPirateAttackEndDelegate);
DECLARE_DELEGATE(FOnPirateGiveDamageDelegate);

UCLASS()
class CAPSTONE_2024_20_API UPirateAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void OnAttackEnd() const;

	UFUNCTION(BlueprintCallable)
	void OnGiveDamage();

	FOnPirateAttackEndDelegate OnPirateAttackEndDelegate;
	FOnPirateGiveDamageDelegate OnPirateGiveDamageDelegate;
};
