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
	/// ���Ѵ밪 (�������� ������ const�� ���������Ƿ� ����)
	/// </summary>
	UPROPERTY(BlueprintReadOnly)
	int32 InfiniteValue = 2147483647;

	/// <summary>
	/// �ش� Ÿ���� Index
	/// </summary>
	UPROPERTY(BlueprintReadWrite)
	FIntPoint Index = FIntPoint(-1);

	/// <summary>
	/// �ش� Ÿ�Ͽ� �����ϱ� ���� Cost
	/// </summary>
	UPROPERTY(BlueprintReadWrite)
	int32 CostToEnterTile = 1;
	
	/// <summary>
	/// ���������� �ش� Ÿ�ϱ��� �ʿ��� Cost
	/// </summary>
	UPROPERTY(BlueprintReadWrite)
	int32 CostFromStart = 2147483647;

	/// <summary>
	/// �ش� Ÿ�Ͽ��� ������ Ÿ�ϱ��� �ʿ��� Heuristics Cost
	/// </summary>
	UPROPERTY(BlueprintReadWrite)
	int32 HeuristicsCostToTarget = 2147483647;

	/// <summary>
	/// �ش� Ÿ�Ͽ� �����ϱ� ���� Ÿ���� Index
	/// </summary>
	UPROPERTY(BlueprintReadWrite)
	FIntPoint PreviousIndex = FIntPoint(-1);
};