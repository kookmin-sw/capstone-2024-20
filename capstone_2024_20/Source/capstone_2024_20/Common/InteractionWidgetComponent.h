#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "InteractionWidgetComponent.generated.h"


class URoundProgressWidget;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CAPSTONE_2024_20_API UInteractionWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UInteractionWidgetComponent();

protected:
	UPROPERTY()
	URoundProgressWidget* RoundProgressWidget;

	FTimerHandle ProgressTimerHandle;
	double PrevTimeSeconds;
	
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	template< class T >
	T* GetRoundProgressWidget() const
	{
		return Cast<T>(RoundProgressWidget);
	}

	UFUNCTION()
	void Show();
	
	UFUNCTION()
	virtual void Hide();
	
	UFUNCTION()
	void StartProgressBar(float Time);

	UFUNCTION()
	void StopProgressBar();
};
