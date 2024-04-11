// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerListController.h"

#include "PlayerListWidgetCreate.h"
#include "PlayerListWidgetModifier.h"
#include "Kismet/GameplayStatics.h"


APlayerListController::APlayerListController()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootComponent);

	static ConstructorHelpers::FClassFinder<UPlayerListWidget>
	BP_PlayerListWidget(TEXT("/Game/WidgetBlueprints/BP_PlayerListWidget.BP_PlayerListWidget_C"));
	PlayerListWigetClass = BP_PlayerListWidget.Class;
	bReplicates = true;
}

void APlayerListController::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerListWidgetCreate* PlayerListWigetCreate = new PlayerListWidgetCreate(GetWorld(), PlayerListWigetClass,
	                                                                           &PlayerListWidget, &PlayerListUpdate);
	PlayerListUpdate = PlayerListWigetCreate;
}

void APlayerListController::PostLogin_Implementation(APlayerState* NewPlayer)
{
	if (PlayerListUpdate)
	{
		PlayerListUpdate->PostLogin(NewPlayer);
	}
}

void APlayerListController::Logout_Implementation(APlayerState* Exiting)
{
	PlayerListUpdate->Logout(Exiting);
}


APlayerListController* APlayerListController::Find(UWorld* World)
{
	TArray<AActor*> FoundActors;
	APlayerListController* PlayerListController = nullptr;
	UGameplayStatics::GetAllActorsOfClass(World, APlayerListController::StaticClass(), FoundActors);
	if (FoundActors.Num() > 0)
	{
		PlayerListController = Cast<APlayerListController>(FoundActors[0]);
	}
	return PlayerListController;
}

void APlayerListController::PostLoginTimer(UWorld* World, APlayerListController** PlayerListController,
	APlayerState* PlayerState)
{
	FTimerHandle TimerHandle;
	const FTimerDelegate MyTimerDelegate = FTimerDelegate::CreateLambda(
		&APlayerListController::PostLoginTimerCallback, PlayerListController, PlayerState);
	World->GetTimerManager().SetTimer(
		TimerHandle,
		MyTimerDelegate,
		2.0f,
		false);
}

void APlayerListController::PostLoginTimerCallback(APlayerListController** PlayerListController,
	APlayerState* PlayerState)
{
	(*PlayerListController)->PostLogin(PlayerState);
}
