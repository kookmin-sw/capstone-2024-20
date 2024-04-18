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

FTransform UGrid::GetTransform() const
{
	return Transform;
}

void UGrid::SetTransform(const FTransform& NewTransform)
{
	Transform = NewTransform;
}

FRotator UGrid::GetRotator() const
{
	return Rotator;
}

void UGrid::SetRotator(const FRotator& NewRotator)
{
	Rotator = NewRotator;
}
