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

TArray<FIntPoint> UGridPathfinding::FindPath(FIntPoint Start, FIntPoint Target, bool bIncludeDiagonals, float Delay)
{
	// Start와 Path를 반대로 저장
	// 결과값을 Reverse하지 않아도 됨
	StartIndex = Target;
	TargetIndex = Start;
	bIncludeDiagonalsDuringPathfinding = bIncludeDiagonals;
	DelayBetweenIterations = Delay;

	ClearDataGeneretedDuringPathfinding();
	if (IsInputDataValid())
	{
		FTimerDelegate PathfindingIteration = FTimerDelegate::CreateLambda(
			[&]() {
				if (DiscoveredTiles.Num() == 0) return;
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
					// Path가 완성된 경우, Path를 반환하고 Pathfinding을 종료
					if (Neighbor.Index == TargetIndex)
					{
						PathfindingResult = GenerateReversedPath();
						OnPathfindingCompleted.Broadcast(PathfindingResult);
						GetWorld()->GetTimerManager().ClearTimer(PathfindingTimeHandle);
					}
				}
			});
		FPathfindingData TilePathData(StartIndex, 1, 0, GetHeuristicsCostBetweenTwoTiles(StartIndex, TargetIndex, bIncludeDiagonalsDuringPathfinding), FIntPoint(-1));
		PushTileInDiscoveredTiles(TilePathData);
		if(DelayBetweenIterations > 0)
		{
			GetWorld()->GetTimerManager().SetTimer(
				PathfindingTimeHandle,
				PathfindingIteration,
				DelayBetweenIterations,
				true
			);
		}
		else
		{

			while ((PathfindingResult.Num() == 0) && DiscoveredTiles.Num() > 0)
			{
				PathfindingIteration.Execute();
			}
		}
	}

	// 더이상 return값이 사용되지 않음
	return PathfindingResult;
}

bool UGridPathfinding::IsInputDataValid()
{
	// 시작 타일과 목적지 타일이 같은 경우
	if (StartIndex == TargetIndex) return false;
	// 시작 타일과 목적지 타일이 올바르지 않은 경우
	if (!(Grid->IsTileWalkable(StartIndex))) return false;
	if (!(Grid->IsTileWalkable(TargetIndex))) return false;
	return true;
}

void UGridPathfinding::PushTileInDiscoveredTiles(FPathfindingData TilePathData)
{
	// 이미 Discovered된 Tile인 경우 새로 갱신할지 안할지 여부를 Check
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
	int32 CurrentTileCost = CalculateTileCost(TilePathData);
	// Discovered Tiles가 빈 경우
	if (DiscoveredTiles.Num() == 0)
	{
		DiscoveredTiles.Add(TilePathData.Index);
		OnPathfindingDataUpdated.Broadcast(TilePathData.Index);
		return;
	}

	// 마지막 Index부터 순회
	// n번째 Cost가 CurrentTileCost보다 작은 index를 처음 만났을 때, 해당 index 뒤에 insert한다.
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
			OnPathfindingDataUpdated.Broadcast(TilePathData.Index);
			return;
		}
	}

	// CurrentTileCost가 가장 작은 경우, 가장 맨 앞에 insert 한다.
	DiscoveredTiles.Insert(TilePathData.Index, 0);
	OnPathfindingDataUpdated.Broadcast(TilePathData.Index);
	
}

int32 UGridPathfinding::CalculateTileCost(FPathfindingData TilePathData)
{
	int32 Cost = TilePathData.CostFromStart + TilePathData.HeuristicsCostToTarget;
	Cost *= 2;
	if (IsDiagonal(TilePathData.Index, TilePathData.PreviousIndex)) 
		Cost++;
	return Cost;
}

bool UGridPathfinding::IsDiagonal(FIntPoint IndexA, FIntPoint IndexB) const
{
	return !GetNeighborIndexes(IndexA).Contains(IndexB) && GetNeighborIndexes(IndexA, true).Contains(IndexB);
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
	PathfindingResult.Empty();
	PathfindingData.Empty();
	DiscoveredTiles.Empty();
	AnalysedTiles.Empty();
	OnPathfindingDataCleared.Broadcast();
}

TArray<FIntPoint> UGridPathfinding::GetNeighborIndexes(FIntPoint Index, bool bIncludeDiagonals) const
{
	TArray<FIntPoint> Neighbors;
	switch (Grid->GetGridShape())
	{
	case EGridShape::Square:
		Neighbors = GetNeighborIndexesForSquare(Index, bIncludeDiagonals);
		break;
	case EGridShape::Triangle:
		// 미구현
		break;
	case EGridShape::Hexagon:
		// 미구현
		break;
	}
	return Neighbors;
}

TArray<FIntPoint> UGridPathfinding::GetNeighborIndexesForSquare(FIntPoint Index, bool bIncludeDiagonals) const
{
	TArray<FIntPoint> Neighbors;
	const TArray<FIntPoint> AdjacentOffsets = bIncludeDiagonals
		? TArray<FIntPoint>({
			{1, 0},
			{0, 1},
			{-1, 0},
			{0, -1},
			{1, 1},
			{-1, 1},
			{-1, -1},
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

