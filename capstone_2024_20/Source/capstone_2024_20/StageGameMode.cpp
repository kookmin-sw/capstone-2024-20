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
	GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red,
	                                 TEXT("LOGOUT"));

	PlayerListController->Logout(Exiting);
}

void AStageGameMode::PostLoginTimer(APlayerController* NewPlayer)
{
	PlayerListController->PostLogin(NewPlayer);
	GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Emerald,
	                                 TEXT("POST LOGIN"));


	FString Name = NewPlayer->PlayerState->GetPlayerName();
	int32 Id = NewPlayer->PlayerState->GetPlayerId();

	GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Emerald,
	                                 FString::Printf(TEXT("name: %s, id: %d"), *Name, Id));
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
	FTimerHandle TimerHandle;
	FTimerDelegate MyTimerDelegate = FTimerDelegate::CreateLambda([NewPlayer, this]()
	{
		PostLoginTimer(NewPlayer);
	});
	GetWorldTimerManager().SetTimer(
		TimerHandle,
		MyTimerDelegate,
		2.0f,
		false);
}
