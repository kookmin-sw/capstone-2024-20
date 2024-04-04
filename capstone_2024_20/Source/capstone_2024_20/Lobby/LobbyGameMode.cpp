// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "LobbyCharacter.h"
#include "LobbyPlayerState.h"

ALobbyGameMode::ALobbyGameMode()
{
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->InputComponent->BindKey(EKeys::T, IE_Pressed,
		                                          this, &ThisClass::GameStart);
	}
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	ALobbyCharacter* LobbyCharacter = Cast<ALobbyCharacter>(NewPlayer->GetCharacter());

	ALobbyPlayerState* LobbyPlayerState = NewPlayer->GetPlayerState<ALobbyPlayerState>();
	LobbyPlayerState->SetInitPlayerNumber(GetNumPlayers());
}

void ALobbyGameMode::GameStart()
{
	HasAuthority()
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
