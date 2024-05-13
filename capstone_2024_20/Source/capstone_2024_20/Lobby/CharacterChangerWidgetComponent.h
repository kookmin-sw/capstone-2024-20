#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "CharacterChangerWidgetComponent.generated.h"


class UCharacterChangeWidget;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CAPSTONE_2024_20_API UCharacterChangerWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UCharacterChangerWidgetComponent();

	UPROPERTY()
	UCharacterChangeWidget* CharacterChangeWidget;
protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
