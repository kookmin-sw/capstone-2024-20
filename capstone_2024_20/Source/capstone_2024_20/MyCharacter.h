
#pragma once

#include "CoreMinimal.h"
#include "MyObject.h"
#include "Common/NamePlateWidgetComponent.h"
#include "GameFramework/Character.h"
#include "Common/HP.h"
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
class CAPSTONE_2024_20_API AMyCharacter : public ACharacter, public IHP
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
		TEXT("Telescope"),
		TEXT("Bed")
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
	FRotator GetMeshRotation();

	void SetEnemyInAttackRange(AEnemy* Enemy);
};
