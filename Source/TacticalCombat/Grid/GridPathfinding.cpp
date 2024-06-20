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

TArray<FIntPoint> UGridPathfinding::GetValidTileNeighbors(FIntPoint Index, bool bIncludeDiagonals)
{
	TArray<FIntPoint> Neighbors;
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
		Neighbors.Add(NeighborIndex);
	}
	return Neighbors;
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
		// 미구현
		break;
	case EGridShape::Hexagon:
		// 미구현
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

