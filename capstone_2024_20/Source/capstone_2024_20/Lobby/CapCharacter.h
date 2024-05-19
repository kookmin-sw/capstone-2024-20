// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ICapPawn.h"
#include "GameFramework/Character.h"
#include "CapCharacter.generated.h"

class ULobbyInteractionWidgetComponent;
class ALobbyPlayerState;
class UCharacterChangerComponent;
class ACapInteractionActor;
class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;

UCLASS()
class CAPSTONE_2024_20_API ACapCharacter : public ACharacter, public IMappingContextGetter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere)
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere)
	UCharacterChangerComponent* CharacterChangerComponent;
	
	UPROPERTY()
	bool bIsMovement = true;

	UPROPERTY(EditAnywhere)
	class ULobbyPlateWidgetComponent* WidgetComponent;

	UPROPERTY(EditAnywhere)
	ALobbyPlayerState* LobbyPlayerState;

	void Init();

public:
	UPROPERTY(EditAnywhere)
	ULobbyInteractionWidgetComponent* InteractionWidgetComponent;

	ACapCharacter();
	FORCEINLINE virtual UInputMappingContext* GetMappingContext() override
	{
		return DefaultMappingContext;
	};

protected:
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);
	void Interact();
	void InteractCancel();
	
public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;


	ACapInteractionActor* CapInteractionActor;
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved,
	                       FVector HitLocation, FVector HitNormal, FVector NormalImpulse,
	                       const FHitResult& Hit) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	void SetIsMovement(bool bNewValue);
	FORCEINLINE bool GetIsMovement() const { return bIsMovement; };

	UFUNCTION()
	void SetReady();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetLocationAndRotation(FVector NewLocation, FRotator NewRotation);
	UFUNCTION(Client, Reliable)
	void ClientRPC_SetLocationAndRotation(FVector NewLocation, FRotator NewRotation);


	UFUNCTION()
	void SetVisibleWigetWithBool(bool IsActive);
private:
	void InitMovement();
};
