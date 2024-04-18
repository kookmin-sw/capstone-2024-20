#include "Map.h"
#include "Grid.h"

UMap::UMap()
{
	// do nothing
}

void UMap::Initialize()
{
	constexpr int32 GridCount = 20; // Todo@autumn - This is a temporary solution, replace it with data.

	Grids.Init(TArray<UGrid*>(), GridCount);
	for (int i = 0; i < GridCount; i++)
	{
		for (int j = 0; j < GridCount; j++)
		{
			Grids[i].Add(NewObject<UGrid>());
		}
	}

	constexpr int MidX = GridCount / 2;
	constexpr int MidY = GridCount / 2;
	
	// 시작 지점에 Obstacle을 생성하지 않도록 하기 위함
	Grids[MidX][MidY]->SetValue(-1);
	Grids[MidX + 1][MidY]->SetValue(-1);
	Grids[MidX - 1][MidY]->SetValue(-1);
	Grids[MidX][MidY + 1]->SetValue(-1);
	Grids[MidX][MidY - 1]->SetValue(-1);
}

void UMap::Divide()
{
	int Width = Grids.Num();
	int Height = Grids[0].Num();

	TQueue<int> LeftBoundQueue;
	TQueue<int> RightBoundQueue;
	TQueue<int> TopBoundQueue;
	TQueue<int> BottomBoundQueue;

	LeftBoundQueue.Enqueue(-1);
	RightBoundQueue.Enqueue(Width);
	TopBoundQueue.Enqueue(-1);
	BottomBoundQueue.Enqueue(Height);
	
	for (int i = 0; i < 10; i++)
	{
		constexpr int MinHeight = 5;
		int LeftBound;
		int RightBound;
		int TopBound;
		int BottomBound;

		LeftBoundQueue.Dequeue(LeftBound);
		RightBoundQueue.Dequeue(RightBound);
		TopBoundQueue.Dequeue(TopBound);
		BottomBoundQueue.Dequeue(BottomBound);
		
		Width = RightBound - LeftBound - 1;
		Height = BottomBound - TopBound - 1;

		if (constexpr int MinWidth = 5; Width < MinWidth || Height < MinHeight)
		{
			continue;
		}
		
		if (Width < Height)
		{
			// Divide horizontally
			
			const int Mid = FMath::RandRange(TopBound + Height * 0.4, BottomBound - Height * 0.4);
			
			for (int x = LeftBound + 1; x < RightBound; x++)
			{
				Grids[x][Mid]->SetValue(1);
			}

			LeftBoundQueue.Enqueue(LeftBound);
			RightBoundQueue.Enqueue(RightBound);
			TopBoundQueue.Enqueue(TopBound);
			BottomBoundQueue.Enqueue(Mid);

			LeftBoundQueue.Enqueue(LeftBound);
			RightBoundQueue.Enqueue(RightBound);
			TopBoundQueue.Enqueue(Mid);
			BottomBoundQueue.Enqueue(BottomBound);
		}
		else
		{
			// Divide vertically
			
			const int Mid = FMath::RandRange(LeftBound + Width * 0.4, RightBound - Width * 0.4);
			
			for (int y = TopBound + 1; y < BottomBound; y++)
			{
				Grids[Mid][y]->SetValue(1);
			}

			LeftBoundQueue.Enqueue(LeftBound);
			RightBoundQueue.Enqueue(Mid);
			TopBoundQueue.Enqueue(TopBound);
			BottomBoundQueue.Enqueue(BottomBound);

			LeftBoundQueue.Enqueue(Mid);
			RightBoundQueue.Enqueue(RightBound);
			TopBoundQueue.Enqueue(TopBound);
			BottomBoundQueue.Enqueue(BottomBound);
		}
	}

	LeftBoundQueue.Empty();
	RightBoundQueue.Empty();
	TopBoundQueue.Empty();
	BottomBoundQueue.Empty();
}

void UMap::Debug()
{
	for (int y = 0; y < Grids.Num(); y++)
	{
		FString RowString;
		for (int x = 0; x < Grids[0].Num(); x++)
		{
			RowString += FString::FromInt(Grids[x][y]->GetValue());
		}
		UE_LOG(LogTemp, Warning, TEXT("%s"), *RowString);
	}
}
