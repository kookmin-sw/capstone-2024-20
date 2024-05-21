#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PirateAnimInstance.generated.h"

DECLARE_DELEGATE(FOnPirateGiveDamageDelegate);

UCLASS()
class CAPSTONE_2024_20_API UPirateAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void OnAttackEnd();

	UFUNCTION(BlueprintCallable)
	void OnGiveDamage();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool bIsAttacking;

	FOnPirateGiveDamageDelegate OnPirateGiveDamageDelegate;
	
private:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
