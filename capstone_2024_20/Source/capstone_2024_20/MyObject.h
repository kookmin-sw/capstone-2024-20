// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyObject.generated.h"

UCLASS()
class CAPSTONE_2024_20_API AMyObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
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
