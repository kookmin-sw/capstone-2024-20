// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerListController.h"

#include "PlayerListWidgetCreate.h"
#include "PlayerListWidgetModifier.h"
#include "GameFramework/GameStateBase.h"
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

	Init();
}

void APlayerListController::Init()
{
	PlayerListWidgetCreate* PlayerListWigetCreate = new PlayerListWidgetCreate(GetWorld(), PlayerListWigetClass,
																		   &PlayerListWidget, &PlayerListUpdate);
	
	PlayerListUpdate = PlayerListWigetCreate;
}

void APlayerListController::MutliRPC_PostLogin_Implementation()
{
	if (PlayerListUpdate)
	{
		PlayerListUpdate->PostLogin(GetWorld()->GetGameState()->PlayerArray);
	}
}

void APlayerListController::MultiRPC_Logout_Implementation(APlayerState* ExitingPlayerState)
{
	if (PlayerListUpdate)
	{
		PlayerListUpdate->Logout(GetWorld()->GetGameState()->PlayerArray, ExitingPlayerState);
	}
}

void APlayerListController::PostLogin()
{
	MutliRPC_PostLogin();
}

void APlayerListController::Logout(AController* Exiting)
{
	MultiRPC_Logout(Exiting->PlayerState);
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

void APlayerListController::PostLoginTimer(UWorld* World, APlayerListController** PlayerListController)
{
	FTimerHandle TimerHandle;

	const FTimerDelegate MyTimerDelegate = FTimerDelegate::CreateLambda(
		&APlayerListController::PostLoginTimerCallback, PlayerListController);

	World->GetTimerManager().SetTimer(
		TimerHandle,
		MyTimerDelegate,
		6.0f,
		false);
}

void APlayerListController::PostLoginTimerCallback(APlayerListController** PlayerListController)
{
	(*PlayerListController)->PostLogin();
}
