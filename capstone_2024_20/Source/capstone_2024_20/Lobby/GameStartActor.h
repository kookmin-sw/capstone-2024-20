// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CapInteractionActor.h"
#include "GameStartActor.generated.h"

class URoundProgressControllerWidget;
class ULevelSequencePlayer;
class ULevelSequence;

UCLASS()
class CAPSTONE_2024_20_API AGameStartActor : public ACapInteractionActor
{
	GENERATED_BODY()

public:
	AGameStartActor();

	UPROPERTY(EditAnywhere)
	ULevelSequence* LevelSequence;
	
	UPROPERTY(EditAnywhere)
	ULevelSequencePlayer* LevelSequencePlayer;
protected:
	virtual void BeginPlay() override;

public:
	virtual void InteractionEnter() override;
	virtual void InteractionLongEnter() override;
	virtual void InteractionExit() override;
	
	UFUNCTION(BlueprintCallable)
	void SetVisibleWidget(UUserWidget* NewUserWidget);
	UPROPERTY()
	UUserWidget* VisibleWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> RoundProgressControllerWidgetClass;

	UPROPERTY(EditAnywhere)
	URoundProgressControllerWidget* RoundProgressControllerWidget;

	UFUNCTION()
	void SkipPressed();

	UFUNCTION()
	void SkipRealesed();

	void Skip();

	FTimerHandle SkipTimerHandle;
public:
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlaySequence();

	UFUNCTION()
	void GameStart();
};
