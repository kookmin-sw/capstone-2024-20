// Fill out your copyright notice in the Description page of Project Settings.


#include "JoinMenuGameState.h"

#include "NetworkService.h"
#include "Kismet/GameplayStatics.h"

void AJoinMenuGameState::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World, ANetworkService::StaticClass(), FoundActors);
	if (FoundActors.Num() > 0)
	{
		NetworkService = Cast<ANetworkService>(FoundActors[0]);
	}
}

void AJoinMenuGameState::Init(ANetworkService* NewNetworkService)
{
	NetworkService = NewNetworkService;
}

void AJoinMenuGameState::SetRoomData(const FRoomData& NewRoomData)
{
	RoomData = NewRoomData;
}

void AJoinMenuGameState::SetRoomName(const FText& RoomName)
{
	RoomData.Name = RoomName.ToString();
}

void AJoinMenuGameState::SetRoomCode(const FText& RoomCode)
{
	RoomData.Code = RoomCode.ToString();
}
