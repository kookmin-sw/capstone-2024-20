// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChatWidget.h"
#include "GameFramework/Actor.h"
#include "ChatService.generated.h"

class UChatWidget;

UCLASS()
class CAPSTONE_2024_20_API AChatService : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<UChatWidget> ChatWidgetClass;

	UPROPERTY(EditAnywhere)
	UChatWidget* ChatWidget;
public:
	// Sets default values for this actor's properties
	AChatService();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void EnableChat();

	UFUNCTION()
	void AddChatLog(EChatType ChatType, const FString& Title,  const FString& NewDetail);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_ReceiveMessage(EChatType ChatType, const FString& Title, const FString& Text);

	void SendNotifyMessage(const FString& Text);
};
