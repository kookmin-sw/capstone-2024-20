// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "MyIngameHUD.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONE_2024_20_API AMyIngameHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> IngameWidgetClass;

private:
	UUserWidget *IngameWidget;
};
