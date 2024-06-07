// Fill out your copyright notice in the Description page of Project Settings.


#include "Action_AddTile.h"
#include "../../PlayerActions.h"
#include "../../../Grid/Grid.h"

void UAction_AddTile::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAction_AddTile::ExecuteAction(FIntPoint Index)
{
	Super::ExecuteAction(Index);
	if(PlayerActions == nullptr) return;
	AGrid* Grid = PlayerActions->GetGrid();
	if (!Grid->IsIndexValid(Index)) 
	{
		FVector GridTileLocation;
		ETileType GridTileType;
		Grid->TraceForGround(Grid->GetTileLocationFromGridIndex(Index), GridTileLocation, GridTileType);
		FTileData Data(
			Index,
			ETileType::Ground,
			FTransform(
				Grid->GetTileRotationFromGridIndex(Index),
				GridTileLocation,
				Grid->GetTileScale()
			)
		);
		Grid->AddGridTile(Data);
	}
}

void UAction_AddTile::BeginPlay()
{
	Super::BeginPlay();
}
