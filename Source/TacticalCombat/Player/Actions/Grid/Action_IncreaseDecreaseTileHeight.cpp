// Fill out your copyright notice in the Description page of Project Settings.


#include "Action_IncreaseDecreaseTileHeight.h"
#include "../../PlayerActions.h"
#include "../../../Grid/Grid.h"

void UAction_IncreaseDecreaseTileHeight::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAction_IncreaseDecreaseTileHeight::ExecuteAction(FIntPoint Index)
{
	Super::ExecuteAction(Index);
	if (PlayerActions == nullptr) return;
	AGrid* Grid = PlayerActions->GetGrid();
	FTileData Data;
	// �ش� Index�� Tile�� �����ϴ� ���, Data�� ������
	if (Grid->GetTileDataFromIndex(Index, Data))
	{
		Data.Transform.SetLocation(
			Data.Transform.GetLocation()
			+ (bIsInCrease ? 1 : -1) * FVector::UpVector * Grid->GetTileSize().Z
		);
		Grid->AddGridTile(
			Data
		);
	}
}

void UAction_IncreaseDecreaseTileHeight::BeginPlay()
{
	Super::BeginPlay();
}
