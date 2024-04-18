#include "Map.h"
#include "Grid.h"

UMap::UMap()
{
	// do nothing
}

void UMap::Initialize()
{
	constexpr int32 GridCount = 20; // Todo@autumn - This is a temporary solution, replace it with data.
	
	for (int x = 0; x < GridCount; x++)
	{
		Grids.Add(TArray<UGrid*>());
		
		for (int y = 0; y < GridCount; y++)
		{
			Grids[x].Add(NewObject<UGrid>());
			Grids[x][y]->SetGridType(EGridType::Empty);
			
			constexpr float GridSize = 10000.0f;
			const float XPos = (x - GridCount / 2) * GridSize + GridSize / 2;
			const float YPos = (y - GridCount / 2) * GridSize + GridSize / 2;
			const FTransform GridTransform = FTransform(FVector(XPos, YPos, 0.0f));
			const FRotator RandRotator = FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f);
			
			Grids[x][y]->SetTransform(GridTransform);
			Grids[x][y]->SetRotator(RandRotator);
		}
	}

	constexpr int MidX = GridCount / 2;
	constexpr int MidY = GridCount / 2;
	
	// 시작 지점에 Obstacle을 생성하지 않도록 하기 위함
	Grids[MidX][MidY]->SetIsProtected(true);
	Grids[MidX + 1][MidY]->SetIsProtected(true);
	Grids[MidX - 1][MidY]->SetIsProtected(true);
	Grids[MidX][MidY + 1]->SetIsProtected(true);
	Grids[MidX][MidY - 1]->SetIsProtected(true);
}

void UMap::CellularAutomata()
{
	for (int x = 0; x < Grids.Num(); x++)
	{
		for (int y = 0; y < Grids[0].Num(); y++)
		{
			if (FMath::RandRange(0, 100) < 50)
			{
				Grids[x][y]->SetGridType(EGridType::Obstacle);
			}
		}
	}
	
	for (int i = 0; i < 3; i ++)
	{
		// 메모리 침범을 방지하기 위해 1부터 시작
		for (int x = 1; x < Grids.Num() - 1; x++)
		{
			for (int y = 1; y < Grids[0].Num() - 1; y++) // 상동
			{
				TArray<UGrid*> Neighbors;
				Neighbors.Add(Grids[x + 1][y]);
				Neighbors.Add(Grids[x - 1][y]);
				Neighbors.Add(Grids[x][y + 1]);
				Neighbors.Add(Grids[x][y - 1]);
				Neighbors.Add(Grids[x + 1][y + 1]);
				Neighbors.Add(Grids[x - 1][y - 1]);
				Neighbors.Add(Grids[x + 1][y - 1]);
				Neighbors.Add(Grids[x - 1][y + 1]);
	
				int ObstacleCount = 0;
				for (const auto Neighbor : Neighbors)
				{
					if (Neighbor->GetGridType() == EGridType::Obstacle)
					{
						ObstacleCount++;
					}
				}
	
				const int EmptyCount = Neighbors.Num() - ObstacleCount;
	
				switch (Grids[x][y]->GetGridType())
				{
				case EGridType::Empty:
					if (ObstacleCount > 4)
					{
						Grids[x][y]->SetGridType(EGridType::Obstacle);
					}
					break;
				case EGridType::Obstacle:
					if (EmptyCount > 4)
					{
						Grids[x][y]->SetGridType(EGridType::Empty);
					}
					break;
				default:
					break;
				}
			}
		}
	}
}

// For debugging, do not remove this function even if it is not used.
void UMap::Debug()
{
	for (int y = 0; y < Grids.Num(); y++)
	{
		FString RowString;
		for (int x = 0; x < Grids[0].Num(); x++)
		{
			RowString += FString::FromInt(static_cast<int32>(Grids[x][y]->GetGridType()));
		}
		UE_LOG(LogTemp, Warning, TEXT("%s"), *RowString);
	}
}

TArray<UGrid*> UMap::GetObstacleGrids() const
{
	TArray<UGrid*> ObstacleGrids;
	
	for (int x = 0; x < Grids.Num(); x++)
	{
		for (int y = 0; y < Grids[0].Num(); y++)
		{
			if (const auto Grid = Grids[x][y]; Grid->GetGridType() == EGridType::Obstacle)
			{
				ObstacleGrids.Add(Grid);
			}
		}
	}

	return ObstacleGrids;
}
