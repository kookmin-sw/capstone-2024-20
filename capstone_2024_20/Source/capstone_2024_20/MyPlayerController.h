// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "IControlStrategy.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "MyCannon.h"
#include "MyCannonBallBox.h"
#include "MyCharacter.h"
#include "MyShip.h"
#include "MyBed.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONE_2024_20_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AMyPlayerController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);

	virtual void OnPossess(APawn* InPawn) override;
	
private:
	UPROPERTY(Category=Input, VisibleAnywhere)
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(Category=Input, VisibleAnywhere)
	UInputMappingContext* CannonMappingContext;

	UPROPERTY(Category=Input, VisibleAnywhere)
	UInputAction* MoveAction;

	UPROPERTY(Category=Input, VisibleAnywhere)
	UInputAction* InteractionAction;
	
	UPROPERTY(Category=Input, VisibleAnywhere)
	UInputAction* ShootAction;

	UPROPERTY(Category=Input, VisibleAnywhere)
	UInputAction* DraggingRotateAction;

	UPROPERTY(Category=Input, VisibleAnywhere)
	UInputAction* AttackAction;
	
public:
	UPROPERTY(Category=UI, VisibleAnywhere)
	class UWidgetComponent* TextWidget;

private:
	AActor* ControlledActor;
	IControlStrategy* CurrentStrategy;
	AMyShip* Ship;
	AMyCharacter* Player;
	AMyCannon* Cannon;
	AMyObject* CurrentHitObject;
	AMyCannonBallBox* CannonBallBox;
	AMyBed* Bed;
	UStaticMesh* CannonBall;
	UEnhancedInputLocalPlayerSubsystem* Subsystem;
	UInputMappingContext* LastMappingContext;

	// 카메라 전환 중인지 체크하는 변수
	bool bIsCameraTransitioning = false;
	
	// 키를 누르고 있는지의 상태를 추적하는 변수
	bool bIsPressingKey = false;

	// 키를 누른 시점부터의 시간을 측정하기 위한 변수
	float PressDuration = 0.0f;

	void Interaction_Pressed();
	void Interaction_Trigger();
	void Interaction_Released();

	
	void DraggingRotate(const FInputActionInstance& Instance);

	
public:
	void Move(const FInputActionInstance& Instance);
	void Interaction(const FInputActionInstance& Instance);
	void Shoot(const FInputActionInstance& Instance);
	void Attack(const FInputActionInstance& Instance);
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_Shoot(AMyCannon* CannonActor);

	UFUNCTION(Server, Reliable)
	void ServerRPC_RotateCannon(AMyCannon* CannonActor, FRotator newRot);

	UFUNCTION(Server, Reliable)
	void ServerRPC_MoveShip_Loc(FVector newLoc);

	UFUNCTION(Server, Reliable)
	void ServerRPC_MoveShip_Rot(float newYaw, float speed);

	UFUNCTION(Server, Reliable)
	void ServerRPC_LoadCannonBall(AMyCannon* CannonActor);

	UFUNCTION(Server, Reliable)
	void ServerRPC_UseCannonBall(AMyCannon* CannonActor);

	UFUNCTION(Server, Reliable)
	void ServerRPC_DragObject(AMyCharacter* user);

	UFUNCTION(Server, Reliable)
	void ServerRPC_DropObject(AMyObject* user, AActor* ship);

	UFUNCTION(Server, Reliable)
	void ServerRPC_RotateDraggingObject(AMyObject* obj, FRotator newRotation);

	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnCarryCannonBall(AMyCharacter* user, AMyCannonBallBox* box);

	UFUNCTION(Server, Reliable)
	void ServerRPC_DestroyCarryCannonBall(AMyCharacter* user);

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerSleep(AMyCharacter* user, bool b, AMyBed* bed);

	UFUNCTION()
	void PlayerSleep();

	UFUNCTION()
	void PlayerAwake();

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerAwake(AMyCharacter* user, bool b, AMyBed* bed);

protected:
	
	enum class ControlMode
	{
		CHARACTER,
		SHIP,
		CANNON,
		TELESCOPE,
		BED
	};
	
	void SetControlMode(ControlMode NewControlMode);
	ControlMode CurrentControlMode=ControlMode::CHARACTER;
	void ViewChange();

	float TargetArmLength;
	FRotator TargetRotation;
	float ChangeSpeed = 5.0f;

	bool flag = true;
};
