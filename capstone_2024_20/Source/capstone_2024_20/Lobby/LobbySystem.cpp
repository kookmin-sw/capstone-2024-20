// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbySystem.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"


// Sets default values
ALobbySystem::ALobbySystem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALobbySystem::BeginPlay()
{
	Super::BeginPlay();
	SceneLobbyWidget = CreateWidget(GetWorld(), LobbyWidgetClass);
	SceneLobbyWidget->AddToViewport();
}

// Called every frame
void ALobbySystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

