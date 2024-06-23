// Fill out your copyright notice in the Description page of Project Settings.


#include "GridPathfinding.h"
#include "Grid.h"

// Sets default values for this component's properties
UGridPathfinding::UGridPathfinding()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGridPathfinding::BeginPlay()
{
	Super::BeginPlay();
	Grid = Cast<AGrid>(GetOwner());
	if (!ensure(Grid != nullptr))
	{
		UE_LOG(LogTemp, Error, TEXT("Grid Pathfinding Actor Component is only used on Grid Actor"));
		return;
	}
}


// Called every frame
void UGridPathfinding::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

TArray<FPathfindingData> UGridPathfinding::GetValidTileNeighbors(FIntPoint Index, bool bIncludeDiagonals)
{
	TArray<FPathfindingData> Neighbors;
	FTileData IndexTileData;
	Grid->GetTileDataFromIndex(Index, IndexTileData);
	TArray <FIntPoint> NeighborIndexes = GetNeighborIndexes(Index, bIncludeDiagonals);
	for (auto NeighborIndex : NeighborIndexes)
	{
		FTileData NeighborTileData;
		Grid->GetTileDataFromIndex(NeighborIndex, NeighborTileData);
		if(!UTileDataUtility::IsTileWalkable(NeighborTileData.Type)) 
			continue;
		if (FMath::Abs(IndexTileData.Transform.GetLocation().Z - NeighborTileData.Transform.GetLocation().Z)> Grid->GetTileSize().Z) 
			continue;

		FPathfindingData NeighborData;
		NeighborData.Index = NeighborIndex;
		NeighborData.PreviousIndex = Index;
		Neighbors.Add(NeighborData);
	}
	return Neighbors;
}

TArray<FIntPoint> UGridPathfinding::FindPath(FIntPoint Start, FIntPoint Target, bool bIncludeDiagonals)
{
	// Start�� Path�� �ݴ�� ����
	// ������� Reverse���� �ʾƵ� ��
	StartIndex = Target;
	TargetIndex = Start;
	bIncludeDiagonalsDuringPathfinding = bIncludeDiagonals;

	ClearDataGeneretedDuringPathfinding();
	if (IsInputDataValid())
	{
		FPathfindingData TilePathData(StartIndex, 1, 0, GetHeuristicsCostBetweenTwoTiles(StartIndex, TargetIndex, bIncludeDiagonalsDuringPathfinding), FIntPoint(-1));
		PushTileInDiscoveredTiles(TilePathData);
		while (DiscoveredTiles.Num() > 0)
		{
			FPathfindingData CurrentTile = PullCheapestTileOutOfDiscoveredTiles();
			TArray<FPathfindingData> CurrentNeighbors = GetValidTileNeighbors(CurrentTile.Index, bIncludeDiagonalsDuringPathfinding);
			for (auto Neighbor : CurrentNeighbors)
			{
				if (!AnalysedTiles.Contains(Neighbor.Index))
				{
					Neighbor.CostFromStart = CurrentTile.CostFromStart + Neighbor.CostToEnterTile;
					Neighbor.HeuristicsCostToTarget = GetHeuristicsCostBetweenTwoTiles(Neighbor.Index, TargetIndex, bIncludeDiagonals);
					Neighbor.PreviousIndex = CurrentTile.Index;
					PushTileInDiscoveredTiles(Neighbor);
				}
				// Path�� �ϼ��� ���, Path�� ��ȯ�ϰ� Pathfinding�� ����
				if(Neighbor.Index == TargetIndex)
					return GenerateReversedPath();
			}
		}
	}

	// Start���� Target������ ��ΰ� �������� ����
	return TArray<FIntPoint>();
}

bool UGridPathfinding::IsInputDataValid()
{
	// ���� Ÿ�ϰ� ������ Ÿ���� ���� ���
	if (StartIndex == TargetIndex) return false;
	// ���� Ÿ�ϰ� ������ Ÿ���� �ùٸ��� ���� ���
	if (!(Grid->IsTileWalkable(StartIndex))) return false;
	if (!(Grid->IsTileWalkable(TargetIndex))) return false;
	return true;
}

void UGridPathfinding::PushTileInDiscoveredTiles(FPathfindingData TilePathData)
{
	// �̹� Discovered�� Tile�� ��� ���� �������� ������ ���θ� Check
	if (DiscoveredTiles.Contains(TilePathData.Index))
	{
		FPathfindingData ExistData = PathfindingData[TilePathData.Index];
		if (TilePathData.CostFromStart < ExistData.CostFromStart)
		{
			DiscoveredTiles.Remove(ExistData.Index);
			PathfindingData.Remove(ExistData.Index);
		}
		else { return; }
	}
	PathfindingData.Add(TilePathData.Index, TilePathData);
	int32 CurrentTileCost = TilePathData.CostFromStart + TilePathData.HeuristicsCostToTarget;
	// Discovered Tiles�� �� ���
	if (DiscoveredTiles.Num() == 0)
	{
		DiscoveredTiles.Add(TilePathData.Index);
		return;
	}

	// ������ Index���� ��ȸ
	// n��° Cost�� CurrentTileCost���� ���� index�� ó�� ������ ��, �ش� index �ڿ� insert�Ѵ�.
	FIntPoint Index;
	FPathfindingData Data;
	int32 Cost;
	for (int i = DiscoveredTiles.Num() - 1; i >= 0; i--)
	{
		Index = DiscoveredTiles[i];
		Data = PathfindingData[Index];
		Cost = Data.CostFromStart + Data.HeuristicsCostToTarget;
		if (Cost < CurrentTileCost)
		{
			DiscoveredTiles.Insert(TilePathData.Index, i + 1);
			return;
		}
	}

	// CurrentTileCost�� ���� ���� ���, ���� �� �տ� insert �Ѵ�.
	DiscoveredTiles.Insert(TilePathData.Index, 0);
	
}

FPathfindingData UGridPathfinding::PullCheapestTileOutOfDiscoveredTiles()
{
	// Pull First Element
	FIntPoint Index = DiscoveredTiles[0];
	DiscoveredTiles.RemoveAt(0);

	// Add Analysed Tiles
	AnalysedTiles.Add(Index);

	return PathfindingData[Index];
}

int32 UGridPathfinding::GetHeuristicsCostBetweenTwoTiles(FIntPoint IndexA, FIntPoint IndexB, bool bDiagonals)
{
	FIntPoint Diff = IndexA - IndexB;
	int32 DiffAbsX = FMath::Abs(Diff.X);
	int32 DiffAbsY = FMath::Abs(Diff.Y);
	int32 Cost = bDiagonals ? FMath::Max(DiffAbsX, DiffAbsY) : DiffAbsX + DiffAbsY;
	return Cost;
}

TArray<FIntPoint> UGridPathfinding::GenerateReversedPath()
{
	TArray<FIntPoint> Path;
	FIntPoint Index = TargetIndex;
	while (Index != StartIndex)
	{
		Path.Add(Index);
		Index = PathfindingData[Index].PreviousIndex;
	}
	Path.Add(StartIndex);
	
	return Path;
}

void UGridPathfinding::ClearDataGeneretedDuringPathfinding()
{
	PathfindingData.Empty();
	DiscoveredTiles.Empty();
	AnalysedTiles.Empty();
}

TArray<FIntPoint> UGridPathfinding::GetNeighborIndexes(FIntPoint Index, bool bIncludeDiagonals)
{
	TArray<FIntPoint> Neighbors;
	switch (Grid->GetGridShape())
	{
	case EGridShape::Square:
		Neighbors = GetNeighborIndexesForSquare(Index, bIncludeDiagonals);
		break;
	case EGridShape::Triangle:
		// �̱���
		break;
	case EGridShape::Hexagon:
		// �̱���
		break;
	}
	return Neighbors;
}

TArray<FIntPoint> UGridPathfinding::GetNeighborIndexesForSquare(FIntPoint Index, bool bIncludeDiagonals)
{
	TArray<FIntPoint> Neighbors;
	const TArray<FIntPoint> AdjacentOffsets = bIncludeDiagonals
		? TArray<FIntPoint>({
			{1, 0},
			{1, 1},
			{0, 1},
			{-1, 1},
			{-1, 0},
			{-1, -1},
			{0, -1},
			{1, -1},
			})
		: TArray<FIntPoint>({
			{1, 0},
			{0, 1},
			{-1, 0},
			{0, -1},
			});
	for (auto Offset : AdjacentOffsets)
	{
		FIntPoint CurrentIndex = Index + Offset;
		if (Grid->IsIndexValid(CurrentIndex))
		{
			Neighbors.Add(CurrentIndex);
		}
	}
	return Neighbors;
}

