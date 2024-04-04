// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridShapeData.h"
#include "TileData.h"
#include "Grid.generated.h"


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

	UFUNCTION()
	void SetLocation(FVector Value);
	UFUNCTION()
	FVector GetLocation() { return GetActorLocation(); }
	UFUNCTION()
	void SetGridOffest(float Value);
	UFUNCTION()
	float GetGridOffest() { return GridOffset; }
	UFUNCTION()
	FVector GetCenterLocation() { return GetSnapGridCenterLocation(); }
	UFUNCTION()
	FVector GetBottomLeftLocation() { return GetGridBottomLeftCornerLocaion(); }
	UFUNCTION()
	void SetTileCount(FIntPoint Value);
	UFUNCTION()
	FVector2D GetTileCount() { return GridTileCount; }
	UFUNCTION()
	void SetTileSize(FVector Value);
	UFUNCTION()
	FVector GetTileSize() { return GridTileSize; }

	FGridShapeData GetCurrentShapeData() { return GridShapeData; }

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector GridLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector GridTileSize = FVector(100, 100, 50);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FIntPoint GridTileCount = FIntPoint(9, 9);

	float GridOffset = 1;

	TMap<FIntPoint, FTileData> GridTiles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EGridShape GridShape = EGridShape::Square;

	FGridShapeData GridShapeData;
	
	void DestoryGrid();
	void AddGridTile(FTileData Tile);
	FVector GetGridBottomLeftCornerLocaion();
	FVector GetSnapGridCenterLocation();
	FVector GetTileLocationFromGridIndex(int IndexX, int IndexY);
	FQuat GetTileRotationFromGridIndex(int IndexX, int IndexY);
	bool TryUpdateInstancedMeshByCurrentShape();
	bool TraceForGround(FVector TraceLocation, float Range, FVector& OutLocation, ETileType& TileType);
	FVector GetLocationFromHits(const TArray<FHitResult>& Hits, ETileType& TileType);
	
};
