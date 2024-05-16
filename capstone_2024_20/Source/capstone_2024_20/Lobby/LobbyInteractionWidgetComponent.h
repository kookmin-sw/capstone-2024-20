#pragma once

#include "CoreMinimal.h"
#include "capstone_2024_20/Common/InteractionWidgetComponent.h"
#include "LobbyInteractionWidgetComponent.generated.h"


class ULobbyRoundProgressWidget;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CAPSTONE_2024_20_API ULobbyInteractionWidgetComponent : public UInteractionWidgetComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	ULobbyRoundProgressWidget* LobbyRoundProgressWidget;
public:
	ULobbyInteractionWidgetComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	
	UFUNCTION()
	void ShowWithText(FString& Key, FString& Explain);
};
