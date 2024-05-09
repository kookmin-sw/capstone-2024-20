#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../01_Network/ReplicatedActor.h"
#include "../Common/HP.h"
#include "Enemy.generated.h"

class AMyCharacter;
class AEnemy;

DECLARE_DELEGATE_OneParam(FEnemyDie, AEnemy*);

UCLASS()
class CAPSTONE_2024_20_API AEnemy : public AReplicatedActor, public IHP
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void BeginPlay() override;
	virtual void Die() override;
	
	void MoveToMyCharacter(const AMyCharacter* MyCharacter);
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;

	FEnemyDie EnemyDieDelegate;
};
