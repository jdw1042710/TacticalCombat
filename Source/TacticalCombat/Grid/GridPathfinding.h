// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Utilities/PathfindingData.h"
#include "GridPathfinding.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPathfindingDataUpdated, FIntPoint, Index);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPathfindingDataCleared);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPathfindingCompleted, TArray<FIntPoint>, Path);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TACTICALCOMBAT_API UGridPathfinding : public UActorComponent
{
	GENERATED_BODY()

public:
	/// <summary>
	/// Pathfinding시 각 Iteration마다 걸리는 Delay Time
	/// </summary>
	float DelayBetweenIterations;

	// Sets default values for this component's properties
	UGridPathfinding();

	UPROPERTY(BlueprintAssignable)
	FOnPathfindingDataUpdated OnPathfindingDataUpdated;

	UPROPERTY(BlueprintAssignable)
	FOnPathfindingDataCleared OnPathfindingDataCleared;

	UPROPERTY(BlueprintAssignable)
	FOnPathfindingCompleted OnPathfindingCompleted;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/// <summary>
	/// 기준 Index에 인접한 타일을 반환
	/// </summary>
	/// <param name="Index"> 기준 Index </param>
	/// <param name="bIncludeDiagonals"> 대각선 타일도 포함하여 값을 반환받을 것인지 여부</param>
	/// <returns> 기준 Index의 Neighbor Tiles </returns>
	UFUNCTION(BlueprintCallable)
	TArray<FPathfindingData> GetValidTileNeighbors(FIntPoint Index, bool bIncludeDiagonals = false);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="Start"> 시작 타일 Index </param>
	/// <param name="Target"> 목적지 타일 Index</param>
	/// <param name="bIncludeDiagonals"> 대각선 이동 허용 여부</param>
	/// <returns> 경로 </returns>
	UFUNCTION(BlueprintCallable)
	TArray<FIntPoint> FindPath(FIntPoint Start, FIntPoint Target, bool bIncludeDiagonals = false, float Delay = 0.0f);

	UFUNCTION(BlueprintCallable)
	TMap<FIntPoint, FPathfindingData> GetPathfindingData() const { return PathfindingData; }
	
	UFUNCTION(BlueprintCallable)
	TArray<FIntPoint> GetDiscoveredTiles() const { return DiscoveredTiles; }

	UFUNCTION(BlueprintCallable)
	TSet<FIntPoint> GetAnalysedTiles() const { return AnalysedTiles; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	friend class UAction_Select_MinCost;

	class AGrid* Grid;
	
	/// <summary>
	/// PathFinding 시작 타일 Index
	/// </summary>
	FIntPoint StartIndex;
	/// <summary>
	/// PathFinding 목표 타일 Index
	/// </summary>
	FIntPoint TargetIndex;
	/// <summary>
	/// PathFinding시 대각선 이동 허용 여부
	/// </summary>
	bool bIncludeDiagonalsDuringPathfinding;

	/// <summary>
	/// Delay있는 Pathfinding시 사용되는 TimeHandler
	/// </summary>
	FTimerHandle PathfindingTimeHandle;


	/// <summary>
	/// Pathfinding 결과
	/// </summary>
	TArray<FIntPoint> PathfindingResult;

	/// <summary>
	/// Pathfinding동안 발견한 (아직 탐색하지 않은) 타일 Index 배열  
	/// 가장 Cost가 큰 Tile부터 작은 타일까지 오름차순 정렬로 구성됨
	/// </summary>
	TArray<FIntPoint> DiscoveredTiles;
	/// <summary>
	/// Pathfinding동안 탐색한 타일 Index 배열 
	/// </summary>
	TSet<FIntPoint> AnalysedTiles;

	/// <summary>
	/// Index값을 Key값으로 한 PathfindingData Map
	/// </summary>
	TMap<FIntPoint, FPathfindingData> PathfindingData;

	/// <summary>
	/// StartIndex, TargetIndex가 유효한 값인지 체크
	/// </summary>
	/// <returns></returns>
	bool IsInputDataValid();

	/// <summary>
	/// Discovered Tiles 배열에 Tile을 추가
	/// </summary>
	/// <param name="TilePathData"> 추가할 Tile의 Pathfinding Data</param>
	void PushTileInDiscoveredTiles(FPathfindingData TilePathData);

	/// <summary>
	/// TilePathData에서 Sorting을 위한 Tile Cost를 계산
	/// </summary>
	/// <param name="TilePathData"></param>
	/// <returns></returns>
	int32 CalculateTileCost(FPathfindingData TilePathData);

	bool IsDiagonal(FIntPoint IndexA, FIntPoint IndexB) const;

	/// <summary>
	/// Discovered Tiles 배열에서 가장 Cost가 작은 타일를 반환
	/// </summary>
	/// <returns> 가장 Cost가 작은 타일의 Pathfinding Data</returns>
	FPathfindingData PullCheapestTileOutOfDiscoveredTiles();

	/// <summary>
	/// 두 Index 사이의 Heuristics Cost를 계산
	/// </summary>
	/// <param name="IndexA"></param>
	/// <param name="IndexB"></param>
	/// <param name="bDiagonals"> 대각선 이동 허용 여부</param>
	/// <returns> IndexA와 IndexB간의 Heuristics Cost </returns>
	int32 GetHeuristicsCostBetweenTwoTiles(FIntPoint IndexA, FIntPoint IndexB, bool bDiagonals);

	/// <summary>
	/// Pathfinding Data를 기반으로 TargetIndex에서 StartIndex까지의 Path를 나타내는 Index배열을 생성
	/// </summary>
	/// <returns> 역순으로 정렬된 Path</returns>
	TArray<FIntPoint> GenerateReversedPath();

	/// <summary>
	/// Pathfinding동안 생성된 private 변수들을 Clear
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void ClearDataGeneretedDuringPathfinding();

	TArray<FIntPoint> GetNeighborIndexes(FIntPoint Index, bool bIncludeDiagonals = false) const;
	TArray<FIntPoint> GetNeighborIndexesForSquare(FIntPoint Index, bool bIncludeDiagonals = false) const;
	
};
