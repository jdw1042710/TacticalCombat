// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utilities/GridShapeData.h"
#include "Utilities/TileData.h"
#include "Grid.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTileDataUpdated, FIntPoint, Index);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGridDestroyed);

class AGridVisualizer;
UCLASS()
class TACTICALCOMBAT_API AGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrid();

	virtual void Tick(float DeltaTime) override;

	void SpawnGrid();

	UFUNCTION(BlueprintCallable)
	void SpawnGrid(const FVector& Location, const FVector& TileSize, const FIntPoint& TileCount, EGridShape Shape, bool bAlwaysSpawn = false);

	UFUNCTION(BlueprintCallable)
	void AddGridTile(const FTileData& Tile);

	UFUNCTION(BlueprintCallable)
	void RemoveGridTile(const FIntPoint& Index);

	UFUNCTION(BlueprintCallable)
	void SetLocation(FVector Value);
	UFUNCTION(BlueprintCallable)
	FVector GetLocation() { return GetActorLocation(); }
	UFUNCTION(BlueprintCallable)
	void SetGridOffest(float Value);
	UFUNCTION(BlueprintCallable)
	float GetGridOffest() { return GridOffset; }
	UFUNCTION(BlueprintCallable)
	FVector GetCenterLocation() { return GetSnapGridCenterLocation(); }
	UFUNCTION(BlueprintCallable)
	FVector GetBottomLeftLocation() { return GetGridBottomLeftCornerLocaion(); }
	UFUNCTION(BlueprintCallable)
	FVector GetTileLocationFromGridIndex(FIntPoint Index);
	UFUNCTION(BlueprintCallable)

	FQuat GetTileRotationFromGridIndex(FIntPoint Index);
	UFUNCTION(BlueprintCallable)
	FVector GetTileScale();

	UFUNCTION(BlueprintCallable)
	FVector GetCursorLocationOnGrid();

	UFUNCTION(BlueprintCallable)
	void SetTileCount(FIntPoint Value);

	UFUNCTION(BlueprintCallable)
	FVector2D GetTileCount() { return GridTileCount; }

	UFUNCTION(BlueprintCallable)
	void SetTileSize(FVector Value);

	UFUNCTION(BlueprintCallable)
	FVector GetTileSize() { return GridTileSize; }

	UFUNCTION(BlueprintCallable)
	UGridPathfinding* GetGridPathfinding() { return GridPathfinding; }

	//Tile Index 및 Data 관련 함수들
	UFUNCTION(BlueprintCallable)
	FIntPoint GetTileIndexUnderCursor();

	UFUNCTION(BlueprintCallable)
	FIntPoint GetTileIndexFromWorldLocation(FVector Location);

	UFUNCTION(BlueprintCallable)
	bool GetTileDataFromIndex(FIntPoint Index, FTileData& Data);

	UFUNCTION(BlueprintCallable)
	bool IsTileWalkable(FIntPoint Index);
	
	UFUNCTION(BlueprintCallable)
	TArray<FIntPoint> GetAllIndexesFromTileData();

	UFUNCTION(BlueprintCallable)
	bool TraceForGround(FVector TraceLocation, FVector& OutLocation, ETileType& TileType);

	UFUNCTION(BlueprintCallable)
	bool IsIndexValid(FIntPoint Index);


	// State 관련 함수들
	UFUNCTION(BlueprintCallable)
	void AddStateToTile(FIntPoint Index, ETileState State);

	UFUNCTION(BlueprintCallable)
	void RemoveStateFromTile(FIntPoint Index, ETileState State);

	UFUNCTION(BlueprintCallable)
	TArray<FIntPoint> GetAllTilesWithStates(ETileState State);

	UFUNCTION(BlueprintCallable)
	void ClearStateFromTiles(ETileState State);

	EGridShape GetGridShape() { return GridShape; }
	FGridShapeData GetCurrentShapeData() { return GridShapeData; }

	UPROPERTY(BlueprintAssignable)
	FOnTileDataUpdated OnTileDataUpdated;

	UPROPERTY(BlueprintAssignable)
	FOnGridDestroyed OnGridDestroyed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	// GridVisualizer Actor
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UChildActorComponent* ChildActor;

	// GridVisualizer
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AGridVisualizer* GridVisualizer;

	//Pathfinding
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UGridPathfinding* GridPathfinding;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector GridLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector GridTileSize = FVector(100, 100, 50);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FIntPoint GridTileCount = FIntPoint(9, 9);

	float GridOffset = 1;

	TMap<FIntPoint, FTileData> GridTiles;

	// ETileState를 가지고있는 타일의 Index배열을 저장하는 Map
	TMap<ETileState, TArray<FIntPoint>> TileStateToIndexes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EGridShape GridShape = EGridShape::Square;

	FGridShapeData GridShapeData;
	
	void DestoryGrid();

	FVector GetGridBottomLeftCornerLocaion();
	FVector GetSnapGridCenterLocation();

	bool TryUpdateInstancedMeshByCurrentShape();

	FVector GetLocationFromHits(const TArray<FHitResult>& Hits, ETileType& TileType);
	
};
