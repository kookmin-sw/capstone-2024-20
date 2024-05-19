#pragma once

#include "CoreMinimal.h"
#include "CharacterType.h"
#include "Common/CharacterTypeFactory.h"
#include "Components/ActorComponent.h"
#include "CharacterChangerComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CAPSTONE_2024_20_API UCharacterChangerComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	ACharacter* OwnerCharacter;

	UPROPERTY(ReplicatedUsing = OnRep_CharacterType)
	TEnumAsByte<ECharacterType> CharacterType;

public:
	UCharacterChangerComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(Server, Reliable)
	void Change(ECharacterType Type);

private:
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Change(ECharacterType Type);

	UFUNCTION()
	void OnRep_CharacterType();

	UFUNCTION()
	void ChangeCharacter(ECharacterType Type);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
