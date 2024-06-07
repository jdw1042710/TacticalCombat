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
	// �ش� Index�� Tile�� �����ϴ� ���, Data�� ������
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
