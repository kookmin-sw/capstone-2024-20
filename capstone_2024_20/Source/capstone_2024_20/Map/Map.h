#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Map.generated.h"

class UGrid;

UCLASS()
class CAPSTONE_2024_20_API UMap : public UObject
{
	GENERATED_BODY()

public:
	UMap();

	void Initialize();
	void Divide();
	void CellularAutomata();
	void Debug();

	TArray<UGrid*> GetObstacleGrids() const;

private:
	TArray<TArray<UGrid*>> Grids;
};
