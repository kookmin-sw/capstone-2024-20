#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Trigger.generated.h"

class ASailingSystem;

UCLASS()
class CAPSTONE_2024_20_API UTrigger : public UObject
{
	GENERATED_BODY()

public:
	UTrigger();
	void Initialize(const FString& Id, ASailingSystem* InSailingSystem);

	bool (*IsTriggered)();

	UPROPERTY()
	ASailingSystem* SailingSystem;
};