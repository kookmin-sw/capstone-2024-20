// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReadyCharacterWidget.h"
#include "capstone_2024_20/Common/NamePlateWidgetComponent.h"
#include "LobbyPlateWidgetComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CAPSTONE_2024_20_API ULobbyPlateWidgetComponent : public UNamePlateWidgetComponent
{
	GENERATED_BODY()

public:
	ULobbyPlateWidgetComponent();

protected:
	virtual void BeginPlay() override;
private:
	UReadyCharacterWidget* LobbyCharacterWidget;
public:
	void ChangeColor(FLinearColor color);

	void SetVisibilityFromBool(bool IsVisible);
};
