// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ILobbyCharacterWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "LobbyCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONE_2024_20_API ULobbyCharacterWidget : public UUserWidget,public ILobbyCharacterWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta=(BindWidget))
	UImage* ReadyImage;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* NameText;
public:
	virtual void ChangeColor(FLinearColor color) override;

	virtual void SetName(FString Name) override;

	virtual void SetVisibilityFromBool(bool IsVisible) override;
};
