// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridShapeData.h"
#include "Grid.generated.h"

UCLASS()
class TACTICALCOMBAT_API AGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrid();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SpawnGrid(const FVector& Location, const FVector& TileSize, const FVector2D& TileCount, EGridShape Shape);

	UFUNCTION()
	void SetLocation(FVector Value);
	UFUNCTION()
	FVector GetLocation() { return GridLocation; }
	UFUNCTION()
	FVector GetCenterLocation() { return GetSnapGridCenterLocation(); }
	UFUNCTION()
	FVector GetBottomLeftLocation() { return GetGridBottomLeftCornerLocaion(); }
	UFUNCTION()
	void SetTileCount(FVector2D Value);
	UFUNCTION()
	FVector2D GetTileCount() { return GridTileCount; }
	UFUNCTION()
	void SetTileSize(FVector Value);
	UFUNCTION()
	FVector GetTileSize() { return GridTileSize; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInstancedStaticMeshComponent* InstancedMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector GridLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector GridTileSize = FVector(100, 100, 100);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D GridTileCount = FVector2D(1, 1);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EGridShape GridShape = EGridShape::Square;

	UDataTable* GridShapeDataTable;
	FGridShapeData* GridShapeData;

	
	FName GetGridShapeName();
	FVector GetGridBottomLeftCornerLocaion();
	FVector GetSnapGridCenterLocation();
	FVector GetTileLocationFromGridIndex(int IndexX, int IndexY);
	FQuat GetTileRotationFromGridIndex(int IndexX, int IndexY);
	bool TryUpdateInstancedMeshByCurrentShape();
};
