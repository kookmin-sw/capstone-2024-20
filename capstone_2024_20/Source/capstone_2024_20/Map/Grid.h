#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Grid.generated.h"

UCLASS()
class CAPSTONE_2024_20_API UGrid : public UObject
{
	GENERATED_BODY()

public:
	UGrid();

	int GetValue() const;
	void SetValue(const int NewValue);
	
	FTransform GetTransform() const;
	void SetTransform(const FTransform& NewTransform);

	FRotator GetRotator() const;
	void SetRotator(const FRotator& NewRotator);
	
private:
	int Value;
	FTransform Transform;
	FRotator Rotator;
};
