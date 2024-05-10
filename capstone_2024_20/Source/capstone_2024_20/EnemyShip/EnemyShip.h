#pragma once

#include "capstone_2024_20/MyShip.h"
#include "../01_Network/ReplicatedActor.h"
#include "../Common/HP.h"
#include "EnemyShip.generated.h"

class AMyShip;
class AEnemy;
class AEnemyShip;

DECLARE_DELEGATE_OneParam(FEnemyShipDie, AEnemyShip*);

UCLASS()
class AEnemyShip : public AReplicatedActor, public IHP
{
	GENERATED_BODY()
	
public:
	AEnemyShip();
	virtual void BeginPlay() override;
	virtual void Die() override;
	void LookAtMyShip(const AMyShip* MyShip);

	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_LookAtMyShip(const AMyShip* MyShip);
	
	AEnemy* SpawnEnemy(AActor* MyShip, const float DeltaTime) const;

	FEnemyShipDie EnemyShipDieDelegate;

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;
	
	inline static float SpawnEnemyTimer = 0.0f;
};
