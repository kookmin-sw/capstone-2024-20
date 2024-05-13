#pragma once

#include "CoreMinimal.h"
#include "MyObject.h"
#include "Common/NamePlateWidgetComponent.h"
#include "GameFramework/Character.h"
#include "Common/HP.h"
#include "MyCharacter.generated.h"

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
	virtual void Die() override;
	
	UPROPERTY()
	AEnemy* EnemyInAttackRange = nullptr;

public:
	UPROPERTY(Replicated)
	bool bIsSleeping = false;
	TArray<FString> ObjectList = {
		TEXT("Cannon"), 
		TEXT("SteelWheel"), 
		TEXT("CannonBallBox"), 
		TEXT("Telescope"),
		TEXT("Bed")
	};
	
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

	void Attack() const;
	
	void SetEnemyInAttackRange(AEnemy* Enemy);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SetEnemyInAttackRange(AEnemy* Enemy);

	UFUNCTION(BlueprintPure)
	float GetHPPercent();
	
};
