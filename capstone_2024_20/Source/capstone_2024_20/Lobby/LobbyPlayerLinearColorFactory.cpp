// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerLinearColorFactory.h"

FLinearColor ULobbyPlayerLinearColorFactory::GetLinearColor(int NumPlayers)
{
	FLinearColor Color = FLinearColor::Black;
	if(NumPlayers == 1)
		Color = FLinearColor::Red;
	else if(NumPlayers == 2)
		Color = FLinearColor::Blue;
	else if(NumPlayers == 3)
		Color = FLinearColor::Yellow;
	else if(NumPlayers == 4)
		Color = FLinearColor::Green;

	return Color;
}
