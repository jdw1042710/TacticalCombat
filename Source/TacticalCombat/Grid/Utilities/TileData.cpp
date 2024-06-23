// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TileData.h"


FTileData::FTileData(FIntPoint IndexValue, ETileType TypeValue, FTransform TransformValue, TArray<ETileState> StateValues)
	:Index(IndexValue), Type(TypeValue), Transform(TransformValue), States(StateValues)
{
}

bool UTileDataUtility::IsTileWalkable(ETileType TileType)
{
	return TileType == ETileType::Ground;
}
