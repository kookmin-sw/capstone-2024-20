// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "CapCharacter.h"
#include "EngineUtils.h"
#include "LobbyCharacter.h"
#include "LobbyGameState.h"
#include "LobbyPlayerListController.h"
#include "LobbyPlayerState.h"
#include "capstone_2024_20/01_Network/InGameRoomInfoWidget.h"
#include "capstone_2024_20/01_Network/NetworkService.h"
#include "Kismet/GameplayStatics.h"

ALobbyGameMode::ALobbyGameMode()
{
	bUseSeamlessTravel = true;
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
	PlayerListController = APlayerListController::Find(GetWorld());

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->InputComponent->BindKey(EKeys::T, IE_Pressed,
		                                          this, &ThisClass::GameStart);
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANetworkService::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		ALobbyGameState* LobbyGameState = GetGameState<ALobbyGameState>();
		LobbyGameState->NetworkService = Cast<ANetworkService>(FoundActors[0]);
	}

	RoomInfoWidget = CreateWidget<UInGameRoomInfoWidget>(GetWorld(), RoomInfoWidgetFactory);
	RoomInfoWidget->AddToViewport();

	const FString RoomCode = GetGameState<ALobbyGameState>()->GetRoomCode().ToString();
	const FString RoomName = GetGameState<ALobbyGameState>()->GetRoomName().ToString();
	RoomInfoWidget->InitSetText(RoomName, RoomCode);

	if (PlayerController)
	{
		PlayerController->InputComponent->BindKey(EKeys::F1, IE_Pressed, RoomInfoWidget, &UInGameRoomInfoWidget::Show);
		PlayerController->InputComponent->BindKey(EKeys::F1, IE_Released, RoomInfoWidget, &UInGameRoomInfoWidget::Hide);
	}
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ALobbyPlayerState* LobbyPlayerState = NewPlayer->GetPlayerState<ALobbyPlayerState>();
	LobbyPlayerState->SetInitPlayerNumber(GetNumPlayers());

	APlayerListController::PostLoginTimer(GetWorld(), &PlayerListController);
	ALobbyPlayerListController::RegisterReadyEventTimer(GetWorld(),
	                                                    &PlayerListController, LobbyPlayerState);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, NewPlayer]()
	{
		SpawnPlayer(NewPlayer);
	}, 3.0f, false);
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	PlayerListController->Logout(Exiting);
}

bool ALobbyGameMode::AllowCheats(APlayerController* P)
{
	return false;
}

void ALobbyGameMode::GameStart()
{
	if (IsReadyAllPlayer() == true)
	{
		GetWorld()->ServerTravel(FString("/Game/Level/level_StageSelect"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Magenta,
		                                 TEXT("모든 플레이어가 준비해야지 시작 할 수 있습니다."));
	}
}

bool ALobbyGameMode::IsReadyAllPlayer() const
{
	for (auto It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		APlayerController* PlayerController = It->Get();
		if (PlayerController == nullptr)
		{
			return false;
		}

		ALobbyPlayerState* LobbyPlayerState = PlayerController->GetPlayerState<ALobbyPlayerState>();
		if (LobbyPlayerState == nullptr)
		{
			return false;
		}

		const bool bIsReady = LobbyPlayerState->IsReady();
		if (bIsReady == false)
		{
			return false;
		}
	}

	return true;
}

void ALobbyGameMode::RestartPlayer(AController* NewPlayer)
{
	if (NewPlayer == nullptr || NewPlayer->IsPendingKillPending())
	{
		return;
	}
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(),TEXT("InitStartPosition"), FoundActors);

	for(AActor* FoundActor: FoundActors)
	{
		RestartPlayerAtPlayerStart(NewPlayer, FoundActor);
		break;
	}
}

void ALobbyGameMode::SpawnPlayer(AController* NewPlayer)
{
	if (GetWorld())
	{
		for (TActorIterator<ACapCharacter> It(GetWorld()); It; ++It)
		{
			ACapCharacter* CapCharacter = *It;
			if (CapCharacter)
			{
				CapCharacter->RefreshNamePlate();
			}
		}
	}

	AActor* PlayerStart = FindPlayerStart(NewPlayer, FString::FromInt(GetNumPlayers()));

	NewPlayer->StartSpot = PlayerStart;
	NewPlayer->GetCharacter()->SetActorLocation(PlayerStart->GetActorLocation());
	NewPlayer->GetCharacter()->SetActorRotation(PlayerStart->GetActorRotation());
}

void ALobbyGameMode::PrintRoomCode()
{
	const FName RoomCode = GetGameState<ALobbyGameState>()->GetRoomCode();
}
