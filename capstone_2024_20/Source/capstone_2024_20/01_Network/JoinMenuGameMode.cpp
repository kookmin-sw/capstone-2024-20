// Fill out your copyright notice in the Description page of Project Settings.


#include "JoinMenuGameMode.h"

#include "JoinMenuGameState.h"
#include "JoinMenuWidget.h"
#include "NetworkService.h"
#include "OnlineSessionSettings.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/ListView.h"
#include "Kismet/GameplayStatics.h"

void AJoinMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	UWorld* World = GetWorld();
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World, ANetworkService::StaticClass(), FoundActors);
	ANetworkService* NetworkService = nullptr;
	if (FoundActors.Num() > 0)
	{
		NetworkService = Cast<ANetworkService>(FoundActors[0]);
	}
	
	const AJoinMenuGameState *LobbyGameState = GetGameState<AJoinMenuGameState>();
	
	if(MainWidget != nullptr)
	{
		MainUI = CreateWidget<UJoinMenuWidget>(World, MainWidget);
		if(MainUI != nullptr)
		{
			MainUI->AddToViewport();
		}
	}

	MainUI->RoomNameTextBox->OnTextChanged.AddDynamic(LobbyGameState, &AJoinMenuGameState::SetRoomName);
	MainUI->RoomCodeTextBox->OnTextChanged.AddDynamic(LobbyGameState, &AJoinMenuGameState::SetRoomCode);
	MainUI->RoomPasswordTextBox->OnTextChanged.AddDynamic(LobbyGameState, &AJoinMenuGameState::SetRoomPassword);

	MainUI->CreateButton->OnClicked.AddDynamic(NetworkService, &ANetworkService::CreateGameSession);
	MainUI->JoinRoomCodeButton->OnClicked.AddDynamic(NetworkService, &ANetworkService::JoinGameSession);
	MainUI->RefreshButton->OnClicked.AddDynamic(this, &ThisClass::OnClickRefreshButton);
	//OnClickRefreshButton();
}

void AJoinMenuGameMode::OnClickRefreshButton()
{
	ANetworkService* NetworkService = GetGameState<AJoinMenuGameState>()->NetworkService;
	NetworkService->AddFindSessionsCompleteDelegate(
		FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::RefreshRoomListUI));

	NetworkService->FindGameSession();
}

void AJoinMenuGameMode::RefreshRoomListUI(bool bWasSuccessful) const
{
	ANetworkService* NetworkService = GetGameState<AJoinMenuGameState>()->NetworkService;
	
	if (bWasSuccessful == false)
	{
		return;
	}

	MainUI->RoomListView->ClearListItems();
	MainUI->RefreshRoomList(NetworkService->GetSessionSearch()->SearchResults);
}