// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyCharacter.h"

#include "UnrealWidgetFwd.h"
#include "Components/WidgetComponent.h"


// Sets default values
ALobbyCharacter::ALobbyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ALobbyCharacter::BeginPlay()
{
	Super::BeginPlay();
	LobbyPlayerState = Cast<ALobbyPlayerState>(GetPlayerState());
	
	ReadyCharacterWidget = Cast<UReadyCharacterWidget>(
		WidgetComponent->GetWidget());

	ReadyCharacterWidget->ChnageColor(FLinearColor::Red);
	ReadyCharacterWidget->SetVisibilityFromBool(false);
}

// Called every frame
void ALobbyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ALobbyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ALobbyCharacter::SetReady()
{
	ReadyCharacterWidget->SetVisibilityFromBool(LobbyPlayerState->IsReady());
}

