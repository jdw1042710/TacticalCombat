// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PathfindingData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FPathfindingData
{
	GENERATED_USTRUCT_BODY()

	FPathfindingData(){}
	FPathfindingData(FIntPoint Index, int32 CostToEnterTile, int32 CostFromStart, int32 HeuristicsCostToTarget, FIntPoint PreviousIndex);

	/// <summary>
	/// 무한대값 (문법상의 이유로 const가 지워졌으므로 주의)
	/// </summary>
	UPROPERTY(BlueprintReadOnly)
	int32 InfiniteValue = 2147483647;

	/// <summary>
	/// 해당 타일의 Index
	/// </summary>
	UPROPERTY(BlueprintReadWrite)
	FIntPoint Index = FIntPoint(-1);

	/// <summary>
	/// 해당 타일에 진입하기 위한 Cost
	/// </summary>
	UPROPERTY(BlueprintReadWrite)
	int32 CostToEnterTile = 1;
	
	/// <summary>
	/// 시작점부터 해당 타일까지 필요한 Cost
	/// </summary>
	UPROPERTY(BlueprintReadWrite)
	int32 CostFromStart = 2147483647;

	/// <summary>
	/// 해당 타일에서 목적지 타일까지 필요한 Heuristics Cost
	/// </summary>
	UPROPERTY(BlueprintReadWrite)
	int32 HeuristicsCostToTarget = 2147483647;

	/// <summary>
	/// 해당 타일에 진입하기 직전 타일의 Index
	/// </summary>
	UPROPERTY(BlueprintReadWrite)
	FIntPoint PreviousIndex = FIntPoint(-1);
};