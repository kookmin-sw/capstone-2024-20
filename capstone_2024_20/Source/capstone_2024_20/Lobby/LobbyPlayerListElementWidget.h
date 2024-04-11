// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ILobbyReadyWidget.h"
#include "capstone_2024_20/01_Network/PlayerListElementWidget.h"
#include "Components/Image.h"
#include "LobbyPlayerListElementWidget.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONE_2024_20_API ULobbyPlayerListElementWidget : public UPlayerListElementWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UImage* Image;
	
	UFUNCTION()
	void ChangeColor(FLinearColor Color);

	UFUNCTION()
	void SetVisibilityFromBool(bool IsVisible);
};
