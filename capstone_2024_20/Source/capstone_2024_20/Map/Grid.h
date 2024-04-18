#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Grid.generated.h"

enum class EGridType : uint8
{
	Empty,
	Obstacle,
	Ship,
	Enemy,
	Event
};

UCLASS()
class CAPSTONE_2024_20_API UGrid : public UObject
{
	GENERATED_BODY()

public:
	UGrid();

	void SetIsProtected(const bool bNewIsProtected);
	
	EGridType GetGridType() const;
	void SetGridType(const EGridType NewGridType);
	
	FTransform GetTransform() const;
	void SetTransform(const FTransform& NewTransform);

	FRotator GetRotator() const;
	void SetRotator(const FRotator& NewRotator);
	
private:
	bool bIsProtected = false;
	EGridType GridType;
	FTransform Transform;
	FRotator Rotator;
};
