// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyCarryCannonBall.h"
#include "MyCharacter.h"
#include "MyObject.h"
#include "MyCannonBallBox.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONE_2024_20_API AMyCannonBallBox : public AMyObject
{
	GENERATED_BODY()
	
	AActor* SpawnedCannonBall;
public:

	//이 클래스는 블루프린트에서 받아옴
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AMyCarryCannonBall> BP_CarryCannonBallClass;
	

	
	UFUNCTION()
	void SpawnCarryCannonBall(AMyCharacter* user);
	
};
