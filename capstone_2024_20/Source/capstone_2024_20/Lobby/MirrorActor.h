// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CapInteractionActor.h"
#include "MirrorActor.generated.h"

class UCameraComponent;
class UArrowComponent;

UCLASS()
class CAPSTONE_2024_20_API AMirrorActor : public ACapInteractionActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(EditAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere)
	ACameraActor* SubCameraActor;
	
public:
	AMirrorActor();

	

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void InteractionLongEnter() override;
	virtual void InteractionEnter() override;
};
