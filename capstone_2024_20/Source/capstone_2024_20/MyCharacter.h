#pragma once

#include "CoreMinimal.h"
#include "MyObject.h"
#include "Common/NamePlateWidgetComponent.h"
#include "GameFramework/Character.h"
#include "Common/HP.h"
#include "MyCharacter.generated.h"

class AMyIngameHUD;
class UCharacterChangerComponent;
class USpringArmComponent;
class AEnemy;

UENUM()
enum class UserState : uint8
{
	NONE,
	CARRYING,
	DRAGGING,
	SLEEPING,
	DEAD
};

UCLASS()
class CAPSTONE_2024_20_API AMyCharacter : public ACharacter, public IHP
{
	GENERATED_BODY()

public:
	AMyCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Category=UI, VisibleAnywhere)
	class UWidgetComponent* TextWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNamePlateWidgetComponent* NamePlateWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> ClearPopUpWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> GameOverPopUpWidgetClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AActor> BP_CannonBallClass;

	UPROPERTY(VisibleAnywhere)
	UCharacterChangerComponent* CharacterChangerComponent;
	
protected:
	bool bIsOverlap = false;

	AMyObject* CurrentHitObject;
	FString CurrentHitObjectName;
	AActor* SpawnedCannonBall;
	AActor* CurrentCarryObject;
	
	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void EndOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
	
private:
	// [begin] IHP interface
	UPROPERTY(Replicated)
	int32 MaxHP = 0;
	
	UPROPERTY(Replicated)
	int32 CurrentHP = 0;
	// [end] IHP interface

	UPROPERTY()
	AMyIngameHUD* MyInGameHUD;
	
	const float ReviveCooldown = 10.0f;
	float CurrentReviveCooldown = 0.0f;
	
	const float AttackCooldown = 5.0f;

	UPROPERTY(Replicated)
	float CurrentAttackCooldown = 0.0f;

	UPROPERTY()
	AEnemy* EnemyInAttackRange = nullptr;

public:
	// [begin] IHP interface
	virtual int32 GetMaxHP() const override;
	virtual int32 GetCurrentHP() const override;
	virtual void SetMaxHP(const int32 NewMaxHP) override;
	virtual void SetCurrentHP(const int32 NewCurrentHP) override;
	virtual void Heal(const int32 HealAmount) override;
	virtual void Damage(const int32 DamageAmount) override;
	virtual void Die() override;
	// [end] IHP interface
	
	void Revive();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Die() const;

	UFUNCTION (NetMulticast, Reliable)
	void Multicast_Revive() const;
	
	UPROPERTY(Replicated)
	bool bIsSleeping = false;
	TArray<FString> ObjectList = {
		TEXT("Cannon"), 
		TEXT("SteelWheel"), 
		TEXT("CannonBallBox"), 
		TEXT("Telescope"),
		TEXT("Bed"),
		TEXT("Upgrade"),
		TEXT("FireEvent")
	};

	UPROPERTY(Replicated)
	UserState CurrentPlayerState = UserState::NONE;

	UserState GetCurrentPlayerState() const;
	void SetPlayerState(UserState NewPlayerState);
	
	UserState GetUserStateNone();
	UserState GetUserStateCarrying();
	UserState GetUserStateDragging();
	UserState GetUserStateSleeping();

	UFUNCTION()
	void SetNamePlate();
	
	UFUNCTION()
	bool GetIsOverLap();

	UFUNCTION(BlueprintCallable)
	bool GetIsSleeping();

	UFUNCTION()
	void SetIsSleeping(bool b);

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetIsSleeping(bool b);

	UFUNCTION()
	void SetTextWidgetVisible(bool b);

	UFUNCTION()
	bool GetTextWidgetVisible();

	UFUNCTION()
	AMyObject* GetCurrentHitObject();

	UFUNCTION()
	FString GetCurrentHitObjectName();

	UFUNCTION()
	void SetCurrentCarryObject(AActor* obj);

	UFUNCTION()
	void DestroyCannonBall();

	UFUNCTION()
	void DragObject();

	UFUNCTION()
	void DropObject(AActor* ship);

	void Attack();

	UFUNCTION(Server, Reliable)
	void ServerRPC_Attack();
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_Attack() const;
	
	void SetEnemyInAttackRange(AEnemy* Enemy);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SetEnemyInAttackRange(AEnemy* Enemy);

	UFUNCTION(BlueprintPure)
	float GetHPPercent();
	
	void ReduceReviveCooldown(float DeltaTime);
	bool CanRevive() const;

	bool IsAttacking() const;
	void ReduceAttackCooldown(float DeltaTime);
	bool CanAttack() const;
};
