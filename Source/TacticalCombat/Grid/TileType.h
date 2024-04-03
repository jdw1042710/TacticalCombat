// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "TileType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ETileType :uint8
{
	None,
	Ground,
	Obstacle,
};
