// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameState.h"

#include "capstone_2024_20/01_Network/NetworkService.h"
#include "capstone_2024_20/01_Network/RoomData.h"

FName ALobbyGameState::GetRoomCode() const
{
	return NetworkService->GetJoiningSessionSetting(RoomTEXT::CODE);
}

FName ALobbyGameState::GetRoomName() const
{
	return NetworkService->GetJoiningSessionSetting(RoomTEXT::NAME);
}