// Fill out your copyright notice in the Description page of Project Settings.


#include "Action_FindPathToTarget.h"
#include "../../PlayerActions.h"
#include "../../../Grid/Grid.h"
#include "../../../Grid/GridPathfinding.h"

void UAction_FindPathToTarget::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAction_FindPathToTarget::ExecuteAction(FIntPoint Index)
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
	FIntPoint SelectedIndex = PlayerActions->GetSelectedTile();
	FIntPoint HoveredIndex = Grid->GetTileIndexUnderCursor();
	Grid->ClearStateFromTiles(ETileState::IsInPath);
	TArray<FIntPoint> Path = GridPathfinding->FindPath(SelectedIndex,HoveredIndex, bIncludeDiagonals);
	for (auto TileIndex : Path)
	{
		Grid->AddStateToTile(TileIndex, ETileState::IsInPath);
	}
	
}

void UAction_FindPathToTarget::BeginPlay()
{
	Super::BeginPlay();
}

void UAction_FindPathToTarget::DestroyComponent(bool bPromoteChildren)
{
	// Destroy Selected Tile
	ExecuteAction(FIntPoint(-1, -1));
	Super::DestroyComponent(bPromoteChildren);
}
