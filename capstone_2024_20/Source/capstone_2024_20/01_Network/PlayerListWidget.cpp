// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerListWidget.h"

void UPlayerListWidget::Add(FString& NewName)
{
	PlayerElements[length]->SetInfo(FString::FromInt(length + 1), NewName);
	length += 1;
}

void UPlayerListWidget::Remove(int32 Index)
{
	PlayerElements[Index]->Clear();
	for (int i = Index; i < length-1; i++)
	{
		int32 Number = i + 1;
		FString Name = PlayerElements[Number]->GetName();

		PlayerElements[i]->SetInfo(FString::FromInt(Number), Name);
	}
	length -= 1;
}

void UPlayerListWidget::Clear()
{
	length = 0;
	for(int i=0;i<PlayerElements.Num();i++)
	{
		PlayerElements[i]->Clear();
	}
}

int32 UPlayerListWidget::GetLength()
{
	return length;
}
