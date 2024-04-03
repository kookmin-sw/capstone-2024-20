#pragma once
#include "capstone_2024_20/MyShip.h"

#include "EnemyShip.generated.h"

class AMyShip;
class AEnemy;

UCLASS()
class AEnemyShip : public AActor
{
	GENERATED_BODY()
	
public:
	AEnemyShip();
	virtual void BeginPlay() override;
	
	void LookAtMyShip(const AMyShip* MyShip);
	void MoveToMyShip(const AMyShip* MyShip);
	AEnemy* SpawnEnemy(AActor* MyShip, const float DeltaTime) const;

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;
	
	inline static float SpawnEnemyTimer = 0.0f;
};
