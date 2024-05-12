#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

UCLASS()
class CAPSTONE_2024_20_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void OnAttackEnd();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool bIsAttacking;
	
private:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
