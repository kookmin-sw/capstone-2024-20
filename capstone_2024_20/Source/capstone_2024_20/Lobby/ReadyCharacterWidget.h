// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ILobbyCharacterWidget.h"
#include "LobbyCharacterWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "ReadyCharacterWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class CAPSTONE_2024_20_API UReadyCharacterWidget : public UUserWidget, public ILobbyCharacterWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	ULobbyCharacterWidget* LobbyCharacterWidget;
public:
	void ChangeColor(FLinearColor color);

	void SetName(FString Name);

	void SetVisibilityFromBool(bool IsVisible);
};
