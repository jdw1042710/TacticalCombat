// Fill out your copyright notice in the Description page of Project Settings.


#include "Action_Select_MinCost.h"
#include "../../PlayerActions.h"
#include "../../../Grid/Grid.h"
#include "../../../Grid/GridPathfinding.h"

void UAction_Select_MinCost::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAction_Select_MinCost::ExecuteAction(FIntPoint Index)
{
	Super::ExecuteAction(Index);
	if (PlayerActions == nullptr) return;
	AGrid* Grid = PlayerActions->GetGrid();
	if (Grid == nullptr) return;
	UGridPathfinding* GridPathfinding = Grid->GetGridPathfinding();
	if (GridPathfinding == nullptr) return;
	GridPathfinding->ClearDataGeneretedDuringPathfinding();
	if (!Grid->IsIndexValid(Index)) return;
	TArray<FIntPoint> Indexes = Grid->GetAllIndexesFromTileData();
	FIntPoint SelectedTile = PlayerActions->GetSelectedTile();
	for (FIntPoint Index : Indexes)
	{
		int32 Cost = GridPathfinding->GetHeuristicsCostBetweenTwoTiles(Index, SelectedTile, bIncludeDiagonals);
		GridPathfinding->PushTileInDiscoveredTiles(
			FPathfindingData
			(
				Index,
				1,
				0,
				Cost,
				FIntPoint(0)
			)
		);
	}
}

void UAction_Select_MinCost::DestroyComponent(bool bPromoteChildren)
{
	Super::DestroyComponent(bPromoteChildren);
}

void UAction_Select_MinCost::BeginPlay()
{
	Super::BeginPlay();

}
