// Fill out your copyright notice in the Description page of Project Settings.


#include "Action_ShowTileNeighbors.h"
#include "../../PlayerActions.h"
#include "../../../Grid/Grid.h"
#include "../../../Grid/GridPathfinding.h"

void UAction_ShowTileNeighbors::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAction_ShowTileNeighbors::ExecuteAction(FIntPoint Index)
{
	Super::ExecuteAction(Index);
	if (PlayerActions == nullptr) 
	{
		UE_LOG(LogTemp, Warning, TEXT("There is no PlayerActions"));
		return;
	}
	AGrid* Grid = PlayerActions->GetGrid();
	if (Grid == nullptr) 
	{ 
		UE_LOG(LogTemp, Warning, TEXT("There is no Grid on PlayerActions"));
		return; 
	}
	UGridPathfinding* GridPathfinding = Grid->GetGridPathfinding();
	FIntPoint HoveredIndex = Grid->GetTileIndexUnderCursor();
	if (Grid->IsIndexValid(Index)) 
	{
		Grid->ClearStateFromTiles(ETileState::Neighbor);
		TArray<FPathfindingData> Neighbors = GridPathfinding->GetValidTileNeighbors(HoveredIndex, bIncludeDiagonals);
		for (auto Neighbor : Neighbors)
		{
			Grid->AddStateToTile(Neighbor.Index, ETileState::Neighbor);
		}
	}
}

void UAction_ShowTileNeighbors::BeginPlay()
{
	Super::BeginPlay();
}

void UAction_ShowTileNeighbors::DestroyComponent(bool bPromoteChildren)
{
	// Destroy Selected Tile
	ExecuteAction(FIntPoint(-1, -1));
	Super::DestroyComponent(bPromoteChildren);
}
