#pragma once

#include "CoreMinimal.h"
#include "MyObject.h"
#include "Common/NamePlateWidgetComponent.h"
#include "GameFramework/Character.h"
#include "Common/HP.h"
#include "MyCharacter.generated.h"

class UPirateAnimInstance;
class UPopupInteraction;
// ReSharper disable once IdentifierTypo
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

	UPROPERTY(Category=UI, VisibleAnywhere)
	UPopupInteraction* PopupInteraction;

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

	UPROPERTY()
	AMyObject* CurrentHitObject;
	
	FString CurrentHitObjectName;

	UPROPERTY()
	AActor* SpawnedCannonBall;

	UPROPERTY()
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

	void GiveDamage();

	UPROPERTY()
	AMyIngameHUD* MyInGameHUD;
	
	const float ReviveCooldown = 10.0f;

	UPROPERTY(Replicated)
	float CurrentReviveCooldown = 0.0f;
	
	const float AttackCooldown = 1.0f;

	UPROPERTY(Replicated)
	float CurrentAttackCooldown = 0.0f;

	float AttackRange = 200.f;

	UPROPERTY()
	UPirateAnimInstance* PirateAnimInstance = nullptr;

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

	TMap<FString, FString> InteractionTextMap = {
		{TEXT("Cannon"), TEXT("대포")},
		{TEXT("SteelWheel"), TEXT("운전")},
		{TEXT("CannonBallBox"), TEXT("포탄 꺼내기")},
		{TEXT("Telescope"), TEXT("망원경")},
		{TEXT("Bed"), TEXT("휴식")},
		{TEXT("Upgrade"), TEXT("업그레이드")},
		{TEXT("FireEvent"), TEXT("불 끄기")}
	};

	UPROPERTY(Replicated)
	UserState CurrentPlayerState = UserState::NONE;

	UserState GetCurrentPlayerState() const;
	void SetPlayerState(UserState NewPlayerState);

	static UserState GetUserStateNone();
	static UserState GetUserStateCarrying();
	static UserState GetUserStateDragging();
	static UserState GetUserStateSleeping();

	UFUNCTION()
	void SetNamePlate() const;
	
	UFUNCTION()
	bool GetIsOverLap() const;

	UFUNCTION(BlueprintCallable)
	bool GetIsSleeping();

	UFUNCTION()
	void SetIsSleeping(bool b);

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetIsSleeping(bool b);

	UFUNCTION()
	void SetTextWidgetVisible(bool b) const;

	UFUNCTION()
	bool GetTextWidgetVisible() const;

	UFUNCTION()
	AMyObject* GetCurrentHitObject() const;

	UFUNCTION()
	FString GetCurrentHitObjectName();

	UFUNCTION()
	void SetCurrentCarryObject(AActor* OBJ);

	UFUNCTION()
	void DestroyCannonBall();

	UFUNCTION()
	void DragObject();

	UFUNCTION()
	void DropObject(AActor* Ship) const;

	void Attack();

	UFUNCTION(Server, Reliable)
	void ServerRPC_Attack();
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_Attack() const;
	
	UFUNCTION(BlueprintPure)
	float GetHPPercent() const;
	
	void ReduceReviveCooldown(float DeltaTime);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReduceReviveCooldown();
	
	bool CanRevive() const;

	bool IsAttacking() const;
	void ReduceAttackCooldown(float DeltaTime);
	bool CanAttack() const;
};
