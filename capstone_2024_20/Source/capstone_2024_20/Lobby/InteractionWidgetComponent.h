#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "InteractionWidgetComponent.generated.h"


class URoundProgressWidget;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CAPSTONE_2024_20_API UInteractionWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

	UPROPERTY()
	URoundProgressWidget* RoundProgressWidget;

	FTimerHandle ProgressTimerHandle;
public:
	UInteractionWidgetComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void Show(FString& Key, FString& Explain);
	
	UFUNCTION()
	void Hide();
	
	UFUNCTION()
	void StartProgressBar(float Time);

	UFUNCTION()
	void StopProgressBar();
};
