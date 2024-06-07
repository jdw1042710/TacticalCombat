// Fill out your copyright notice in the Description page of Project Settings.


#include "Action_SetTileType.h"
#include "../../PlayerActions.h"
#include "../../../Grid/Grid.h"

void UAction_SetTileType::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAction_SetTileType::ExecuteAction(FIntPoint Index)
{
	Super::ExecuteAction(Index);
	if (PlayerActions == nullptr) return;
	AGrid* Grid = PlayerActions->GetGrid();
	FTileData Data;
	// 해당 Index에 Tile이 존재하는 경우, Data를 가져옴
	if (Grid->GetTileDataFromIndex(Index, Data))
	{
		Data.Type = TileType;
		Grid->AddGridTile(Data);
	}
}

void UAction_SetTileType::BeginPlay()
{
	Super::BeginPlay();
}
