// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PathfindingData.h"

FPathfindingData::FPathfindingData(FIntPoint Index, int32 CostToEnterTile, int32 CostFromStart, int32 HeuristicsCostToTarget, FIntPoint PreviousIndex)
	: Index(Index), CostToEnterTile(CostToEnterTile), CostFromStart(CostFromStart), HeuristicsCostToTarget(HeuristicsCostToTarget), PreviousIndex(PreviousIndex)
{

}
