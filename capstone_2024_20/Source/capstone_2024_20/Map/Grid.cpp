// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"

UGrid::UGrid(): Value(0)
{
	// do nothing
}

int UGrid::GetValue() const
{
	return Value;
}

void UGrid::SetValue(const int NewValue)
{
	if (Value == -1)
	{
		return;
	}

	Value = NewValue;
}
