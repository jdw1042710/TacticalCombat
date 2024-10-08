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
	/// Pathfinding�� �� Iteration���� �ɸ��� Delay Time
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
	/// ���� Index�� ������ Ÿ���� ��ȯ
	/// </summary>
	/// <param name="Index"> ���� Index </param>
	/// <param name="bIncludeDiagonals"> �밢�� Ÿ�ϵ� �����Ͽ� ���� ��ȯ���� ������ ����</param>
	/// <returns> ���� Index�� Neighbor Tiles </returns>
	UFUNCTION(BlueprintCallable)
	TArray<FPathfindingData> GetValidTileNeighbors(FIntPoint Index, bool bIncludeDiagonals = false);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="Start"> ���� Ÿ�� Index </param>
	/// <param name="Target"> ������ Ÿ�� Index</param>
	/// <param name="bIncludeDiagonals"> �밢�� �̵� ��� ����</param>
	/// <returns> ��� </returns>
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
	/// PathFinding ���� Ÿ�� Index
	/// </summary>
	FIntPoint StartIndex;
	/// <summary>
	/// PathFinding ��ǥ Ÿ�� Index
	/// </summary>
	FIntPoint TargetIndex;
	/// <summary>
	/// PathFinding�� �밢�� �̵� ��� ����
	/// </summary>
	bool bIncludeDiagonalsDuringPathfinding;

	/// <summary>
	/// Delay�ִ� Pathfinding�� ���Ǵ� TimeHandler
	/// </summary>
	FTimerHandle PathfindingTimeHandle;


	/// <summary>
	/// Pathfinding ���
	/// </summary>
	TArray<FIntPoint> PathfindingResult;

	/// <summary>
	/// Pathfinding���� �߰��� (���� Ž������ ����) Ÿ�� Index �迭  
	/// ���� Cost�� ū Tile���� ���� Ÿ�ϱ��� �������� ���ķ� ������
	/// </summary>
	TArray<FIntPoint> DiscoveredTiles;
	/// <summary>
	/// Pathfinding���� Ž���� Ÿ�� Index �迭 
	/// </summary>
	TSet<FIntPoint> AnalysedTiles;

	/// <summary>
	/// Index���� Key������ �� PathfindingData Map
	/// </summary>
	TMap<FIntPoint, FPathfindingData> PathfindingData;

	/// <summary>
	/// StartIndex, TargetIndex�� ��ȿ�� ������ üũ
	/// </summary>
	/// <returns></returns>
	bool IsInputDataValid();

	/// <summary>
	/// Discovered Tiles �迭�� Tile�� �߰�
	/// </summary>
	/// <param name="TilePathData"> �߰��� Tile�� Pathfinding Data</param>
	void PushTileInDiscoveredTiles(FPathfindingData TilePathData);

	/// <summary>
	/// TilePathData���� Sorting�� ���� Tile Cost�� ���
	/// </summary>
	/// <param name="TilePathData"></param>
	/// <returns></returns>
	int32 CalculateTileCost(FPathfindingData TilePathData);

	bool IsDiagonal(FIntPoint IndexA, FIntPoint IndexB) const;

	/// <summary>
	/// Discovered Tiles �迭���� ���� Cost�� ���� Ÿ�ϸ� ��ȯ
	/// </summary>
	/// <returns> ���� Cost�� ���� Ÿ���� Pathfinding Data</returns>
	FPathfindingData PullCheapestTileOutOfDiscoveredTiles();

	/// <summary>
	/// �� Index ������ Heuristics Cost�� ���
	/// </summary>
	/// <param name="IndexA"></param>
	/// <param name="IndexB"></param>
	/// <param name="bDiagonals"> �밢�� �̵� ��� ����</param>
	/// <returns> IndexA�� IndexB���� Heuristics Cost </returns>
	int32 GetHeuristicsCostBetweenTwoTiles(FIntPoint IndexA, FIntPoint IndexB, bool bDiagonals);

	/// <summary>
	/// Pathfinding Data�� ������� TargetIndex���� StartIndex������ Path�� ��Ÿ���� Index�迭�� ����
	/// </summary>
	/// <returns> �������� ���ĵ� Path</returns>
	TArray<FIntPoint> GenerateReversedPath();

	/// <summary>
	/// Pathfinding���� ������ private �������� Clear
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void ClearDataGeneretedDuringPathfinding();

	TArray<FIntPoint> GetNeighborIndexes(FIntPoint Index, bool bIncludeDiagonals = false) const;
	TArray<FIntPoint> GetNeighborIndexesForSquare(FIntPoint Index, bool bIncludeDiagonals = false) const;
	
};
