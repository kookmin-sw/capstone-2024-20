#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

DECLARE_DELEGATE(FOnGiveDamageDelegate);

UCLASS()
class CAPSTONE_2024_20_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void OnAttackEnd();

	UFUNCTION(BlueprintCallable)
	void OnGiveDamage();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool bIsAttacking;

	FOnGiveDamageDelegate OnGiveDamageDelegate;
	
private:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
