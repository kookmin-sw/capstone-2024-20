// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "GameMapsSettings.h"
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

	FIsReadyChanged IsReadyChanged;
	IsReadyChanged.BindDynamic(LobbyCharacter, &ALobbyCharacter::ALobbyCharacter::SetReady);
	LobbyPlayerState->SetIsReadyChanged(IsReadyChanged);
}

void ALobbyGameMode::GameStart()
{
	if (IsReadyAllPlayer() == true)
	{
		GetWorld()->ServerTravel(FString("/Game/Level/level_StageSelect"));
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
