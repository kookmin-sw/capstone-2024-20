
#pragma once

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "CoreMinimal.h"
#include "MyObject.h"
#include "Camera/CameraComponent.h"
#include "Common/NamePlateWidgetComponent.h"
#include "GameFramework/Character.h"
#include "UObject/ObjectRename.h"
#include "MyCharacter.generated.h"


class USpringArmComponent;
class AEnemy;

UENUM()
enum class UserState : uint8
{
	NONE,
	CARRYING,
	DRAGGING
};

UCLASS()
class CAPSTONE_2024_20_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
private:

	UPROPERTY(Category=Character, VisibleAnywhere)
	USpringArmComponent* M_SpringArmComponent;
	
	UPROPERTY(Category=Character, VisibleAnywhere)
	UCameraComponent* M_CameraComponent;
	
	UPROPERTY(Category=Character, VisibleAnywhere)
	UStaticMeshComponent* M_MeshComponent;


public:
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
	
protected:

	unsigned int PlayerMaxHP = 10;
	unsigned int PlayerHP = PlayerMaxHP;
	
	bool bIsChanging=false;
	float TargetArmLength;
	FVector TargetLocation;
	FRotator TargetRotation;
	float ChangeSpeed = 5.0f;
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
	UPROPERTY(Replicated)
	FRotator MeshRotation;

	UPROPERTY()
	AEnemy* EnemyInAttackRange = nullptr;
	
public:

	TArray<FString> ObjectList = {
		TEXT("Cannon"), 
		TEXT("SteelWheel"), 
		TEXT("CannonBallBox"), 
		TEXT("Telescope")
	};
	
	
	UserState CurrentPlayerState = UserState::NONE;
	
	void SetPlayerState(UserState NewPlayerState);
	
	UserState GetUserStateNone();
	UserState GetUserStateCarrying();
	UserState GetUserStateDragging();

	UFUNCTION(Server, Reliable)
	void ServerRPC_MeshRotation(FRotator NewRotation);

	UFUNCTION()
	void SetNamePlate();
	
	UFUNCTION()
	bool GetIsOverLap();

	UFUNCTION()
	void SetTextWidgetVisible(bool b);

	UFUNCTION()
	bool GetTextWidgetVisible();

	UFUNCTION()
	void SetIsChanging(float length, FVector Loc, FRotator rot, bool b);

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

	UFUNCTION()
	unsigned int GetPlayerHP();

	UFUNCTION()
	void SetPlayerHP(unsigned int hp);
	
	UFUNCTION()
	void IncreaseHP(int plusHP);

	UFUNCTION()
	void DecreaseHP(unsigned int minusHP);

	UFUNCTION()
	unsigned int GetPlayerMaxHP();

	UFUNCTION()
	void SetPlayerMaxHP(unsigned int hp);
	
	UFUNCTION()
	void IncreaseMaxHP(int plusHP);

	UFUNCTION()
	void DecreaseMaxHP(int minusHP);

	UFUNCTION()
	void PlayerDead();

	UFUNCTION()
	FRotator GetMeshRotation();

	void SetEnemyInAttackRange(AEnemy* Enemy);
};
