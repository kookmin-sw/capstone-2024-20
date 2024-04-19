#include "Grid.h"

UGrid::UGrid(): GridType(EGridType::Empty)
{
	// do nothing
}

void UGrid::SetIsProtected(const bool bNewIsProtected)
{
	bIsProtected = bNewIsProtected;
}

EGridType UGrid::GetGridType() const
{
	return GridType;
}

void UGrid::SetGridType(const EGridType NewGridType)
{
	if (bIsProtected)
	{
		return;
	}

	GridType = NewGridType;
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
