// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Common/HP.h"
#include "MyShip.generated.h"

UCLASS()
class CAPSTONE_2024_20_API AMyShip : public APawn, public IHP
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyShip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	
public:
	UPROPERTY(Category=Character, VisibleAnywhere)
    	UStaticMeshComponent* M_MeshComponent;
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SetShipLocation(FVector newLoc);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void Upgrade();
	float GetMoveSpeed() const;

	UPROPERTY(Replicated)
	FRotator TargetRotation;

	UPROPERTY(Replicated)
	float MoveSpeed = 600.0f; // Todo@autumn replace with data table

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	AActor* Camera_Character;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	AActor* Camera_Ship;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	AActor* Camera_Telescope;

	
};
