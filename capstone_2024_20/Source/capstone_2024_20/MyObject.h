#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyObject.generated.h"

UCLASS()
class CAPSTONE_2024_20_API AMyObject : public AActor
{
	GENERATED_BODY()
	
public:	
	AMyObject();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(Replicated)
	bool IsDragging;

protected:
	UPROPERTY(Replicated)
	bool CanBeOperated;

	bool CanBeOperatedOnServer = false;
	
public:
	bool GetIsDragging();
	void SetIsDragging(bool b);
	
	UPROPERTY(Replicated)
	FRotator TargetRotation;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_TurnOffCollision();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_TurnOnCollision();

	void Interact();
	void InteractOnServer();
	
	virtual void Operate();
	virtual void OperateOnServer();
};
