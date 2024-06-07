// Fill out your copyright notice in the Description page of Project Settings.


#include "Action_SelectTile.h"
#include "../../PlayerActions.h"
#include "../../../Grid/Grid.h"

void UAction_SelectTile::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAction_SelectTile::ExecuteAction(FIntPoint Index)
{
	Super::ExecuteAction(Index);
	if(PlayerActions == nullptr) return;
	AGrid* Grid = PlayerActions->GetGrid();
	FIntPoint SelectedTile = PlayerActions->GetSelectedTile();
	// Check Two Indexes Are Difference
	if (SelectedTile != Index)
	{
		// Change SelectedTile
		Grid->RemoveStateFromTile(SelectedTile, ETileState::Selected);
		PlayerActions->SetSelectedTile(Index);
		Grid->AddStateToTile(Index, ETileState::Selected);
	}
	else
	{
		// Remove SelectedTile
		Grid->RemoveStateFromTile(SelectedTile, ETileState::Selected);
		PlayerActions->SetSelectedTile(FIntPoint(-1, -1));
	}
}

void UAction_SelectTile::BeginPlay()
{
	Super::BeginPlay();
}

void UAction_SelectTile::DestroyComponent(bool bPromoteChildren)
{
	// Destroy Selected Tile
	ExecuteAction(FIntPoint(-1, -1));
	Super::DestroyComponent(bPromoteChildren);
}
