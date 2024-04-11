// Fill out your copyright notice in the Description page of Project Settings.


#include "StageGameMode.h"
#include "StageSelectController.h"
#include "01_Network/InGameRoomInfoWidget.h"
#include "01_Network/InGameStateTest.h"
#include "01_Network/NetworkService.h"
#include "01_Network/PlayerListController.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

class AInGameStateTest;

AStageGameMode::AStageGameMode()
{
}

void AStageGameMode::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow,
	                                 FString(TEXT("Init진출")));

	PlayerListController = APlayerListController::Find(GetWorld());
	
	InitRoomInfo();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStageSelectController::StaticClass(), FoundActors);
	if (FoundActors.Num() > 0)
	{
		StageSelectController = Cast<AStageSelectController>(FoundActors[0]);
	}


}

void AStageGameMode::InitRoomInfo()
{
	UWorld* World = GetWorld();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World, ANetworkService::StaticClass(), FoundActors);
	if (FoundActors.Num() > 0)
	{
		AInGameStateTest* InGameStateTest = GetGameState<AInGameStateTest>();
		InGameStateTest->NetworkService = Cast<ANetworkService>(FoundActors[0]);
	}

	RoomInfoWidget = CreateWidget<UInGameRoomInfoWidget>(GetWorld(), RoomInfoWidgetFactory);
	RoomInfoWidget->AddToViewport(100);

	const FString RoomCode = GetGameState<AInGameStateTest>()->GetRoomCode().ToString();
	const FString RoomName = GetGameState<AInGameStateTest>()->GetRoomName().ToString();
	RoomInfoWidget->InitSetText(RoomName, RoomCode);

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->InputComponent->BindKey(EKeys::C, IE_Pressed, RoomInfoWidget, &UInGameRoomInfoWidget::Show);
		PlayerController->InputComponent->BindKey(EKeys::C, IE_Released, RoomInfoWidget, &UInGameRoomInfoWidget::Hide);
	}
}

void AStageGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	PlayerListController->Logout(Exiting->GetPlayerState<APlayerState>());
}

void AStageGameMode::PostLoginTimer(APlayerController* NewPlayer)
{
	PlayerListController->PostLogin(NewPlayer->GetPlayerState<APlayerState>());
}

void AStageGameMode::PrintRoomCode()
{
	const FName RoomCode = GetGameState<AInGameStateTest>()->GetRoomCode();
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue,
	                                 FString::Printf(TEXT("룸 코드: %s"), *RoomCode.ToString()));

	UE_LOG(LogTemp, Warning, TEXT("Test"));
}

void AStageGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	APlayerListController::PostLoginTimer(GetWorld(), &PlayerListController, NewPlayer->GetPlayerState<APlayerState>());
}
