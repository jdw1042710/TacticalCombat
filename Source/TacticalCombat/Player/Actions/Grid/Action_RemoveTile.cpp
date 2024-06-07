// Fill out your copyright notice in the Description page of Project Settings.


#include "Action_RemoveTile.h"
#include "../../PlayerActions.h"
#include "../../../Grid/Grid.h"

void UAction_RemoveTile::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAction_RemoveTile::ExecuteAction(FIntPoint Index)
{
	Super::ExecuteAction(Index);
	if(PlayerActions == nullptr) return;
	AGrid* Grid = PlayerActions->GetGrid();
	if (Grid->IsIndexValid(Index)) 
	{
		Grid->RemoveGridTile(Index);
	}
}

void UAction_RemoveTile::BeginPlay()
{
	Super::BeginPlay();
}
